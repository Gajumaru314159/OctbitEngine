using Common.Linq;
using Common.Log;
using Common.Tree;
using CommonView.History;
using CommonView.Menu;
using Livet;
using OctbitEngine.Runtime;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.Windows.Input;
using System.Windows.Media.Imaging;

namespace OctbitEditor
{
    public class EntityMock : IEntity
    {
        public EntityMock(string name)
        {
            Name = name;
        }
        public IScene Scene => throw new NotImplementedException();

        public string Name { get; set; }
        public bool IsActive { get; set; } = true;
        public bool IsVisible { get; set; } = true;
        public bool IsStatic { get; set; }

        public IEntity? Parent { get; set; }
        public EntityMock? _parent;

        public IReadOnlyList<IEntity> Children => m_children;
        private List<EntityMock> m_children = new();

        public IReadOnlyList<IComponent> Components => throw new NotImplementedException();

        public ITransformComponent Transform => throw new NotImplementedException();

        public bool AddChild(IEntity child)
        {
            throw new NotImplementedException();
        }

        public IComponent? AddComponent(string type)
        {
            throw new NotImplementedException();
        }

        public T? AddComponent<T>() where T : class,IComponent
        {
            throw new NotImplementedException();
        }

        public IComponent? GetComponent(string type)
        {
            throw new NotImplementedException();
        }

        public T? GetComponent<T>() where T : class, IComponent
        {
            throw new NotImplementedException();
        }

        public IComponent[] GetComponents(string type)
        {
            throw new NotImplementedException();
        }

        public T[] GetComponents<T>() where T : class, IComponent
        {
            throw new NotImplementedException();
        }

        public bool SetParent(IEntity? parent)
        {
            if (parent is not EntityMock cparent)
            {
                _parent?.m_children.Remove(this);
                _parent = null;
                return true;
            }

            if (cparent.AnyAncestor(i => i._parent, i => i == this)) return false;
            _parent?.m_children.Remove(this);
            _parent = cparent;
            _parent.m_children.Add(this);
            return true;
        }
    }


    public class OutlinerItem : ViewModel
    {
        internal static BitmapImage FolderIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/folder.png"));
        internal static BitmapImage EntityIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/entity.png"));

        public OutlinerItem(IEntity entity)
        {
            Entity = entity;
            Parent = null;
            IsNameEditting.Subscribe(i => Log.Info($"IsNameEditting:{i}"));
        }

        public IEntity Entity { get; }

        public OutlinerItem? Parent { get; set; }

        public bool SetParent(OutlinerItem? parent)
        {
            // 循環
            if (parent?.Parent.AnyAncestor(i => i.Parent, i => i==parent)??false) return false;

            if (Parent != null)
            {
                Parent.Children.Remove(this);
            }

            Parent = parent;
            Parent?.Children.Add(this);

            Entity.SetParent(parent?.Entity);

            return false;
        }


        public string Name
        {
            get => Entity.Name;
            set
            {
                if (Name == value) return;
                var oldValue = Name;
                History.Record(
                    $"エンティティの名前を {value} に変更",
                    () => { Entity.Name = value; RaisePropertyChanged(); },
                    () => { Entity.Name = oldValue; RaisePropertyChanged(); }
                );
            }
        }

        enum HierarchyStatus
        {
            Active,
            Visible,
            Static
        }
        private void UpdateHierarchyStatus(HierarchyStatus mode)
        {
            switch (mode)
            {
                case HierarchyStatus.Active:
                    RaisePropertyChanged(nameof(IsActive));
                    RaisePropertyChanged(nameof(ActiveIconOpacity));
                    break;
                case HierarchyStatus.Visible:
                    RaisePropertyChanged(nameof(IsVisible));
                    RaisePropertyChanged(nameof(VisibleIconOpacity));
                    break;
                case HierarchyStatus.Static:
                    break;
            }

            foreach (var child in Children)
            {
                child.UpdateHierarchyStatus(mode);
            }
        }


        public bool IsActive
        {
            get => Entity.IsActive;
            set
            {
                if (IsActive == value) return;
                var oldValue = IsActive;
                History.Record(
                    $"エンティティ({Name})のIsActiveを{value}に変更",
                    () => { Entity.IsActive = value; UpdateHierarchyStatus(HierarchyStatus.Active); },
                    () => { Entity.IsActive = oldValue; UpdateHierarchyStatus(HierarchyStatus.Active); }
                );
            }
        }
        public bool IsVisible
        {
            get => Entity.IsVisible;
            set
            {
                if (IsVisible == value) return;
                var oldValue = IsVisible;
                History.Record(
                    $"エンティティ({Name})のIsVisibleを{value}に変更",
                    () => { Entity.IsVisible = value; UpdateHierarchyStatus(HierarchyStatus.Visible); },
                    () => { Entity.IsVisible = oldValue; UpdateHierarchyStatus(HierarchyStatus.Visible); }
                );
            }
        }

        public double ActiveIconOpacity
            => this.AllAncestor(i => i?.Parent, i => i.IsActive) ? 1.0 : 0.5;
        public double VisibleIconOpacity
            => this.AllAncestor(i => i?.Parent, i => i.IsVisible) ? 1.0 : 0.5;

        public ReactivePropertySlim<bool> IsExpanded { get; } = new(true);
        public ReactivePropertySlim<bool> IsSelected { get; } = new(false);
        public ReactivePropertySlim<bool> IsFiltered { get; } = new(false);
        public ReactivePropertySlim<bool> IsMatched { get; } = new(false);
        public ReactivePropertySlim<bool> IsNameEditting { get; } = new(false);
        public BitmapImage Icon => EntityIcon;
        public ObservableCollection<OutlinerItem> Children { get; } = new();

    }


    public class OutlinerVM : TabBase
    {
        public OutlinerVM(IWorld world)
            : base("Outliner")
        {
            World = world;

            MenuItems = new DynamicGroupItem("Root");

            GenerateMenuItems();
            InitializeCommands();

            SelectedItems.CollectionChanged += (sender, e) =>
            {
                RaisePropertyChanged(nameof(SelectionInfo));
            };

            Filter.Subscribe(_ => { Children.ForEach(i=> UpdateFilter(i)); });
        }

        private bool MatchFilter(OutlinerItem item)
        {
            return item.Name.Contains(Filter.Value) && !string.IsNullOrEmpty(Filter.Value);
        }

        private bool UpdateFilter(OutlinerItem item)
        {
            // TODO async処理
            bool matched = MatchFilter(item);
            bool visible = matched || string.IsNullOrEmpty(Filter.Value);
            foreach (var child in item.Children)
            {
                visible |= UpdateFilter(child);
            }

            item.IsMatched.Value = matched;
            item.IsFiltered.Value = !visible;

            return visible;
        }


        private void InitializeCommands()
        {
            CreateEntityCommand = new DelegateCommand(CreateEntity);
            DeleteEntityCommand = new DelegateCommand(DeleteEntity);
            EditEntityNameCommand = new DelegateCommand(EditEntityName);
        }

        private void GenerateMenuItems()
        {
            MenuItems.AddCommand("Cut", "Ctrl+X", CutEntity).Icon = OutlinerItem.EntityIcon;
            MenuItems.AddCommand("Copy", "Ctrl+C", CutEntity).Icon = OutlinerItem.EntityIcon;
            MenuItems.AddCommand("Paste", "Ctrl+V", CutEntity);
            MenuItems.AddSeparator();
            MenuItems.AddCommand("Rename", "F2",CutEntity);
            MenuItems.AddCommand("Duplicate","Ctrl+D", CutEntity);
            MenuItems.AddCommand("Delete","Delete", DeleteEntity);
            MenuItems.AddSeparator();
            MenuItems.AddCommand("Create Empty", "Ctrl+N", CreateEntity);
            {
                var group = MenuItems.AddGroup("3D Object");
                group.AddCommand("Cube", CutEntity);
                group.AddCommand("Sphere", CutEntity);
                group.AddSeparator();
                group.AddCommand("Capsule", CutEntity);
                {
                    var group2 = group.AddGroup("3D Object");
                    group2.AddCommand("Cube", CutEntity);
                    group2.AddCommand("Sphere", CutEntity);
                    group2.AddSeparator();
                    group2.AddCommand("Capsule", CutEntity);
                }
            }
        }

        public void EditEntityName()
        {
            SelectedItems.ForEach(i=>i.IsNameEditting.Value = i == SelectedItems.Last());
        }

        private void CreateEntity()
        {

            var parent = SelectedItems.FirstOrNull();
            var item = new OutlinerItem(new EntityMock("New Entity") { Name="New Entity" });

            History.Record(
                "エンティティを作成",
                () =>
                {
                    if (parent==null)
                    {
                        Children.Add(item);
                    }
                    else
                    {
                        item.SetParent(parent);
                    }

                    // TODO ツリーの更新、もしくはEntity総数の変更をトリガーにする
                    RaisePropertyChanged(nameof(SelectionInfo));
                },
                () =>
                {
                    if (item.Parent==null)
                    {
                        Children.Remove(item);
                    }
                    item.SetParent(null);
                    RaisePropertyChanged(nameof(SelectionInfo));
                }
            );
            
        }
        private void DeleteEntity()
        {
            var roots = SelectedItems.SelectRoot(i => i.Parent, i => i.Children);
            var items = roots.Select(i => (parent:i.Parent,node:i)).ToList();

            History.Record(
                "エンティティを削除",
                () =>
                {
                    foreach(var i in items)
                    {
                        if (i.parent==null)
                        {
                            Children.Remove(i.node);
                        }
                        else
                        {
                            i.node.SetParent(null);
                        }
                    }
                    RaisePropertyChanged(nameof(SelectionInfo));
                },
                () =>
                {
                    foreach (var i in items)
                    {
                        if (i.parent==null)
                        {
                            Children.Add(i.node);
                        }
                        else
                        {
                            i.node.SetParent(i.parent);
                        }
                    }
                    RaisePropertyChanged(nameof(SelectionInfo));
                }
            );
        }

        private void CutEntity()
        {
            // TODO ヒストリ対応

            var roots = SelectedItems.SelectRoot(i => i.Parent, i => i.Children);

            foreach (var root in roots)
            {
                if (root.Parent==null)
                {
                    Children.Remove(root);
                }
                else
                {
                    root.Parent?.Children.Remove(root);
                }
            }

            // TODO クリップボードにコピー
        }

        // テキストによるフィルタ
        public ReactivePropertySlim<string> Filter { get; } = new(string.Empty);
        public ObservableCollection<OutlinerItem> Children { get; } = new();

        public ObservableCollection<OutlinerItem> SelectedItems { get; set; } = new();

        public string SelectionInfo => $"{SelectedItems.Count}/{Children.Sum(i=>i.DepthFirst(i=>i.Children).Count())} selected";

        public DynamicGroupItem MenuItems { get; private set; }

        // コマンド
        public ICommand? CreateEntityCommand { get; private set; }
        public ICommand? DeleteEntityCommand { get; private set; }
        public ICommand? EditEntityNameCommand { get; private set; }

        private IWorld World { get; init; }
    }
}

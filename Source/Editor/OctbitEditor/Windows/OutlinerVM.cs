using System.Collections.ObjectModel;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using CommonView.Menu;
using Common.Log;
using Reactive.Bindings;
using CommonView.History;
using OctbitEngine.Runtime;
using Livet;
using Common.Tree;

namespace OctbitEditor
{
    public class EntityMock : IEntity
    {
        public EntityMock()
        {

        }
        public IScene Scene => throw new NotImplementedException();

        public string Name { get; set; } = string.Empty;
        public bool IsActive { get; set; } = true;
        public bool IsVisible { get; set; } = true;
        public bool IsStatic { get; set; }

        public bool IsActiveInHierarchy => Parent.AllAncestor<IEntity>(i => i?.Parent, i => i.IsActive);

        public bool IsVisibleInHierarchy => Parent.AllAncestor<IEntity>(i => i?.Parent, i => i.IsVisible);

        public bool IsStaticInHierarchy => Parent.AllAncestor<IEntity>(i => i?.Parent, i => i.IsStatic);

        public IEntity? Parent { get; private set; }

        public IReadOnlyList<IEntity> Children => m_children;
        private List<IEntity> m_children = new();

        public IReadOnlyList<IComponent> Components => m_components;
        private List<IComponent> m_components = new();

        public ITransformComponent Transform => throw new NotImplementedException();

        public IComponent? AddComponent(string type)
        {
            throw new NotImplementedException();
        }

        public T? AddComponent<T>() where T : IComponent
        {
            throw new NotImplementedException();
        }

        public IComponent? GetComponent(string type)
        {
            throw new NotImplementedException();
        }

        public T? GetComponent<T>() where T : IComponent
        {
            throw new NotImplementedException();
        }

        public IComponent[] GetComponents(string type)
        {
            throw new NotImplementedException();
        }

        public T[] GetComponents<T>() where T : IComponent
        {
            throw new NotImplementedException();
        }

        public bool SetParent(IEntity? iparent)
        {
            // 不正な型
            if (iparent is not EntityMock parent) return false;

            // 循環
            if (iparent.Parent.AnyAncestor(i => i.Parent, i => i==parent)) return false;

            if (Parent is EntityMock oldParent)
            {
                oldParent.m_children.Remove(this);
            }

            parent.m_children.Add(this);
            Parent = parent;

            return false;
        }

        public bool AddChild(IEntity child)
        {
            return child.SetParent(this);
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
        }

        public IEntity Entity { get; }

        public OutlinerItem? Parent { get; set; }

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
            => Entity.IsActiveInHierarchy ? 1.0 : 0.5;
        public double VisibleIconOpacity
            => Entity.IsVisibleInHierarchy ? 1.0 : 0.5;

        public ReactiveProperty<bool> IsExpanded { get; } = new(true);
        public ReactiveProperty<bool> IsSelected { get; } = new(false);
        public BitmapImage Icon => EntityIcon;
        public ObservableCollection<OutlinerItem> Children { get; } = new();

    }

    public class OutlinerVM
    {
        public OutlinerVM()
        {
            var entity = new EntityMock();
            entity.AddChild(new EntityMock() { Name="Root" });
            entity.AddChild(new EntityMock() { Name="Root" });
            entity.AddChild(new EntityMock() { Name="Root" });
            entity.Children[0].AddChild(new EntityMock() { Name="Root" });  
            entity.Children[0].AddChild(new EntityMock() { Name="Root" });

            void visit(OutlinerItem? parent,ObservableCollection<OutlinerItem> dst,IEnumerable<IEntity> src)
            {
                foreach (var s in src)
                {
                    var child = new OutlinerItem(s) { Parent = parent };
                    dst.Add(child);
                    visit(child, child.Children, s.Children);
                }
            }

            visit(null,Children, entity.Children);

            MenuItems = new DynamicGroupItem("Root");
            GenerateMenuItems();
        }

        private void GenerateMenuItems()
        {
            MenuItems.AddCommand("Cut", "Ctrl+X", CutEntity).Icon = OutlinerItem.EntityIcon;
            MenuItems.AddCommand("Copy", "Ctrl+C", CutEntity).Icon = OutlinerItem.EntityIcon;
            MenuItems.AddCommand("Paste", "Ctrl+V", CutEntity);
            MenuItems.AddSeparator();
            MenuItems.AddCommand("Rename", CutEntity);
            MenuItems.AddCommand("Duplicate", CutEntity);
            MenuItems.AddCommand("Delete", DeleteEntity);
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

        private void CreateEntity()
        {
            if (SelectedItems.Count == 0) return;
            var entity = new EntityMock() { Name="New Entity" };
            entity.SetParent(SelectedItems[0].Entity);

            SelectedItems[0].Children.Add(new(entity) { Parent = SelectedItems[0] });
        }
        private void DeleteEntity()
        {
            if (SelectedItems.Count == 0) return;
            var item = SelectedItems[0];
            item.Parent?.Children.Remove(item);
        }

        private void CutEntity()
        {
            Log.Info("CutEntity");


            var set = SelectedItems.ToHashSet();
            var list = new List<OutlinerItem>();

            foreach (var item in SelectedItems)
            {
                bool isAncestor = false;
                var parent = item.Parent;
                while (parent != null)
                {
                    if (set.Contains(parent))
                    {
                        isAncestor = true;
                        break;
                    }
                    parent = parent.Parent;
                }
                if (isAncestor) continue;
                list.Add(item);
            }

            foreach (var item in list)
            {
                item.Name = "Cutted";
            }

        }

        // テキストによるフィルタ
        public ReactiveProperty<string> Filter { get; } = new(string.Empty);
        public ObservableCollection<OutlinerItem> Children { get; } = new();

        public ObservableCollection<OutlinerItem> SelectedItems { get; set; } = new();


        public DynamicGroupItem MenuItems { get; private set; }
    }
}

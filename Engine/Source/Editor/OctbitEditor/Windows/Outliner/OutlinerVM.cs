using Common.Linq;
using Common.Log;
using Common.Tree;
using CommonView.History;
using CommonView.Menu;
using Livet;
using OctbitEngine.Asset;
using OctbitEngine.Runtime;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Reactive.Linq;
using System.Windows.Input;
using System.Windows.Media.Imaging;

namespace OctbitEditor
{

    public class SceneMock : IScene
    {
        public IWorld World => throw new NotImplementedException();

        public string Name { get; set; } = string.Empty;

        public IAssetFile? File => throw new NotImplementedException();

        public bool IsActive { get; set; } = true;
        public bool IsVisible { get; set; } = true;

        public IScene? Parent { get; set; }

        public IReadOnlyList<IScene> Children => m_children;
        private List<IScene> m_children = new();

        public IReadOnlyList<IEntity> Entities => m_entities;
        private List<IEntity> m_entities = new();

        public bool AddChild(IScene child)
        {
            throw new NotImplementedException();
        }

        public IEntity CreateEntity()
        {
            throw new NotImplementedException();
        }

        public bool SetParent(IScene? parent)
        {
            throw new NotImplementedException();
        }
    }

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


    public class OutlinerVM : TabBase
    {
        public OutlinerVM(IWorld world)
            : base("Outliner")
        {
            World = world;

            MenuItems = new DynamicGroupItem("Root");

            GenerateMenuItems();
            InitializeCommands();

            // ルートシーン
            var scenes = new List<SceneOutlinerItem>();
            for (int i = 0; i<10; ++i)
            {
                scenes.Add(new SceneOutlinerItem(new SceneMock() { Name=$"Root Scene {i}" }));
            }
            Children = new(scenes);

            // SelectionInfoは頻繁に更新する必要はないのでThrottleをかける
            Observable.FromEventPattern<NotifyCollectionChangedEventHandler,NotifyCollectionChangedEventArgs>
                ( h => SelectedItems.CollectionChanged += h,h=> SelectedItems.CollectionChanged -= h)
                .Throttle(TimeSpan.FromMilliseconds(100))
                .Subscribe(e => RaisePropertyChanged(nameof(SelectionInfo)));

            // TODO 更新時間のコンフィグ公開
            Filter
                .Throttle(TimeSpan.FromMilliseconds(300))
                .Subscribe(_ => { Children.ForEach(i=> UpdateFilter(i)); });

        }

        private bool MatchFilter(OutlinerItemBase item)
        {
            return item.Name.Contains(Filter.Value) && !string.IsNullOrEmpty(Filter.Value);
        }

        private bool UpdateFilter(OutlinerItemBase item)
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
            CreateSceneCommand = new DelegateCommand(CreateSceneTest);
            CreateFolderCommand = new DelegateCommand(CreateFolder);
            CreateEntityCommand = new DelegateCommand(CreateEntity);
            DeleteEntityCommand = new DelegateCommand(DeleteEntity);
            EditEntityNameCommand = new DelegateCommand(EditEntityName);
        }

        private void GenerateMenuItems()
        {
            MenuItems.AddCommand("Cut", "Ctrl+X", CutEntity);
            MenuItems.AddCommand("Copy", "Ctrl+C", CutEntity);
            MenuItems.AddCommand("Paste", "Ctrl+V", CutEntity);
            MenuItems.AddSeparator();
            MenuItems.AddCommand("Rename", "F2",CutEntity);
            MenuItems.AddCommand("Duplicate","Ctrl+D", CutEntity);
            MenuItems.AddCommand("Delete","Delete", DeleteEntity);
            MenuItems.AddSeparator();
            MenuItems.AddCommand("_Create Empty", "Ctrl+N", CreateEntity);
            {
                var group = MenuItems.AddGroup("_3D Object");
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

        private void CreateSceneTest()
        {
            var item = new SceneOutlinerItem(new SceneMock() { Name="New Scene" });

            History.Record(
                "シーンを作成",
                () =>
                {
                    Children.Add(item);

                    // TODO ツリーの更新、もしくはEntity総数の変更をトリガーにする
                    RaisePropertyChanged(nameof(SelectionInfo));
                },
                () =>
                {
                    Children.Remove(item);
                    RaisePropertyChanged(nameof(SelectionInfo));
                }
            );
        }
        private void CreateFolder()
        {
            var parent = SelectedItems.FirstOrNull();
            if (!(parent is SceneOutlinerItem || parent is FolderOutlinerItem)) return;
            if (parent is null) return;

            var item = new FolderOutlinerItem() { Name="New Folder" };

            History.Record(
                "エンティティを作成",
                () =>
                {
                    item.SetParent(parent);

                    // TODO ツリーの更新、もしくはEntity総数の変更をトリガーにする
                    RaisePropertyChanged(nameof(SelectionInfo));
                },
                () =>
                {
                    item.SetParent(null);
                    RaisePropertyChanged(nameof(SelectionInfo));
                }
            );
        }

        private void CreateEntity()
        {
            var parent = SelectedItems.FirstOrNull();
            if (!(parent is SceneOutlinerItem || parent is FolderOutlinerItem || parent is EntityOutlinerItem)) return;
            if (parent is null) return;

            var item = new EntityOutlinerItem(new EntityMock("New Entity") { Name="New Entity" });

            History.Record(
                "エンティティを作成",
                () =>
                {
                    item.SetParent(parent);

                    // TODO ツリーの更新、もしくはEntity総数の変更をトリガーにする
                    RaisePropertyChanged(nameof(SelectionInfo));
                },
                () =>
                {
                    item.SetParent(null);
                    RaisePropertyChanged(nameof(SelectionInfo));
                }
            );            
        }
        private void DeleteEntity()
        {
            var roots = SelectedItems.SelectRoot(i => i.Parent);
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

            var roots = SelectedItems.SelectRoot(i => i.Parent);

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
        public ObservableCollection<OutlinerItemBase> Children { get; } = new();

        public ObservableCollection<OutlinerItemBase> SelectedItems { get; set; } = new();

        public string SelectionInfo => $"{SelectedItems.Count}/{Children.Sum(i=>i.DepthFirst(i=>i.Children).Count())} selected";

        public DynamicGroupItem MenuItems { get; private set; }

        // コマンド
        public ICommand? CreateSceneCommand { get; private set; }
        public ICommand? CreateFolderCommand { get; private set; }
        public ICommand? CreateEntityCommand { get; private set; }
        public ICommand? DeleteEntityCommand { get; private set; }
        public ICommand? EditEntityNameCommand { get; private set; }

        private IWorld World { get; init; }
    }
}

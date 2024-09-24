using Common.Tree;
using CommonView.History;
using Livet;
using OctbitEngine.Runtime;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.Windows.Media.Imaging;

namespace OctbitEditor
{

    public abstract class OutlinerItemBase : ViewModel
    {
        internal static BitmapImage DefaultIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/folder.png"));
        public BitmapImage Icon { get; protected init; }
        public string TypeName { get; protected init; }
        public ObservableCollection<OutlinerItemBase> Children { get; } = new();

        public ReactivePropertySlim<bool> IsExpanded { get; } = new(true);
        public ReactivePropertySlim<bool> IsSelected { get; } = new(false);
        public ReactivePropertySlim<bool> IsNameEditting { get; } = new(false);
        public OutlinerItemBase? Parent { get; protected set; }

        public virtual bool IsActive { get; set; }
        public virtual bool IsVisible { get; set; }
        public double ActiveIconOpacity
            => this.AllAncestor(i => i?.Parent, i => i.IsActive) ? 1.0 : 0.5;
        public double VisibleIconOpacity
            => this.AllAncestor(i => i?.Parent, i => i.IsVisible) ? 1.0 : 0.5;

        protected OutlinerItemBase()
        {
            Icon = DefaultIcon;
            TypeName = "Unknown";
        }

        public virtual bool SetParent(OutlinerItemBase? parent)
        {
            return false;
        }


        protected enum HierarchyStatus
        {
            Active,
            Visible,
            Static
        }
        protected void UpdateHierarchyStatus(HierarchyStatus mode)
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
    }


    public class SceneOutlinerItem : OutlinerItemBase
    {
        internal static BitmapImage SceneIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/folder.png"));
        public SceneOutlinerItem(IScene scene)
        {
            Icon = SceneIcon;
            TypeName = "Scene";
            Scene = scene;
        }

        public IScene Scene { get; }
        public string Name
        {
            get => Scene.Name;
            set
            {
                if (Name == value) return;
                var oldValue = Name;
                History.Record(
                    $"シーンの名前を {value} に変更",
                    () => { Scene.Name = value; RaisePropertyChanged(); },
                    () => { Scene.Name = oldValue; RaisePropertyChanged(); }
                );
            }
        }


        public override bool IsActive
        {
            get => Scene.IsActive;
            set
            {
                if (IsActive == value) return;
                var oldValue = IsActive;
                History.Record(
                    $"シーン({Name})のIsActiveを{value}に変更",
                    () => { Scene.IsActive = value; UpdateHierarchyStatus(HierarchyStatus.Active); },
                    () => { Scene.IsActive = oldValue; UpdateHierarchyStatus(HierarchyStatus.Active); }
                );
            }
        }
        public override bool IsVisible
        {
            get => Scene.IsVisible;
            set
            {
                if (IsVisible == value) return;
                var oldValue = IsVisible;
                History.Record(
                    $"エンティティシーン({Name})のIsVisibleを{value}に変更",
                    () => { Scene.IsVisible = value; UpdateHierarchyStatus(HierarchyStatus.Visible); },
                    () => { Scene.IsVisible = oldValue; UpdateHierarchyStatus(HierarchyStatus.Visible); }
                );
            }
        }

        public override bool SetParent(OutlinerItemBase? parent)
        {
            return false;
        }
    }
    public class EntityOutlinerItem : OutlinerItemBase
    {
        internal static BitmapImage EntityIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/entity.png"));
        public EntityOutlinerItem(IEntity entity)
        {
            Icon = EntityIcon;
            TypeName = "Entity";
            Entity = entity;
        }

        public IEntity Entity { get; }
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
        public override bool IsActive
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
        public override bool IsVisible
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
        public override bool SetParent(OutlinerItemBase? parent)
        {
            return false;
        }
    }
    public class FolderOutlinerItem : OutlinerItemBase
    {
        internal static BitmapImage FolderIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/folder.png"));
        public string Name { get; set; } = "New Folder";
        public FolderOutlinerItem()
        {
            Icon = FolderIcon;
            TypeName = "Folder";
        }

        public override bool SetParent(OutlinerItemBase? parent)
        {
            return false;
        }

    }


    public class OutlinerItemTest
    {
        public void Test(IWorld world)
        {
            // TODO Parent設定
            OutlinerItemBase createEntity(IEntity entity)
            {
                var item = new EntityOutlinerItem(entity);

                foreach (var child in entity.Children)
                {
                    item.Children.Add(createEntity(child));
                }

                return item;
            }

            OutlinerItemBase createScene(IScene scene)
            {
                var item = new SceneOutlinerItem(scene);

                foreach (var child in scene.Children)
                {
                    item.Children.Add(createScene(child));
                }

                foreach (var entity in scene.Entities)
                {
                    item.Children.Add(createEntity(entity));
                }

                return item;
            }


            var items = world.Scenes.Select(i => createScene(i));




        }
    }

}

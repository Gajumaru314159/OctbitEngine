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

namespace OctbitEditor
{
    public class EntityMock : IEntity
    {
        public EntityMock()
        {

        }
        public IScene Scene => throw new NotImplementedException();

        public string Name { get; set; } = string.Empty;
        public bool IsActive { get; set; }
        public bool IsVisible { get; set; }
        public bool IsStatic { get; set; }

        public bool IsActiveInHierarchy => true;

        public bool IsVisibleInHierarchy => true;

        public bool IsStaticInHierarchy => true;

        public IEntity? Parent => null;

        public IReadOnlyList<IEntity> Children => m_children;
        private List<IEntity> m_children = new();

        public IReadOnlyList<IComponent> Components => m_components;
        private List<IComponent> m_components= new();

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

        public bool SetParent(IEntity parent)
        {
            throw new NotImplementedException();
        }
    }




    public class OutlinerItem : ViewModel
    {
        internal static BitmapImage FolderIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/folder.png"));
        internal static BitmapImage EntityIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/entity.png"));

        public OutlinerItem(IEntity entity) {
            Entity = entity;
            Parent = null;
        }

        public IEntity Entity { get; }

        public OutlinerItem? Parent { get; }

        public string Name
        {
            get => Entity.Name;
            set
            {
                if(Name == value) return;
                var oldValue = Name;
                History.Record(
                    "エンティティの名前を変更",
                    () => { Entity.Name = value; RaisePropertyChanged(); },
                    () => { Entity.Name = oldValue; RaisePropertyChanged(); }
                );
            }
        }
        public bool IsActive
        {
            get => Entity.IsActive;
            set
            {
                if(IsActive == value) return;
                var oldValue = IsActive;
                History.Record(
                    "エンティティのIsActiveを変更",
                    () => { Entity.IsActive = value; RaisePropertyChanged(); RaisePropertyChanged(nameof(ActiveIconOpacity)); },
                    () => { Entity.IsActive = oldValue; RaisePropertyChanged(); RaisePropertyChanged(nameof(ActiveIconOpacity)); }
                );
            }
        }
        public bool IsVisible
        {
            get => Entity.IsVisible;
            set
            {
                if(IsVisible == value) return;
                var oldValue = IsVisible;
                History.Record(
                    "エンティティのIsVisibleを変更",
                    () => { Entity.IsVisible = value; RaisePropertyChanged(); RaisePropertyChanged(nameof(VisibleIconOpacity)); },
                    () => { Entity.IsVisible = oldValue; RaisePropertyChanged(); RaisePropertyChanged(nameof(VisibleIconOpacity)); }
                );
            }
        }

        public double ActiveIconOpacity
            => Entity.IsActiveInHierarchy ? 1.0 : 0.5;
        public double VisibleIconOpacity
            => Entity.IsVisibleInHierarchy ? 1.0 : 0.5;

        public ReactiveProperty<bool> IsExpanded{ get; } = new(true);
        public ReactiveProperty<bool> IsSelected { get; } = new(false);
        public BitmapImage Icon => EntityIcon;
        public ObservableCollection<OutlinerItem> Children { get; } = new();

    }

    public class OutlinerVM
    {
        public OutlinerVM()
        {
            Children.Add(new(new EntityMock()) { Name = "Root" });
            Children.Add(new(new EntityMock()) { Name = "Test" });
            Children.Add(new(new EntityMock()) { Name = "Sample" });
            Children[0].Children.Add(new(new EntityMock()) { Name = "Sample" });
            Children[0].Children.Add(new(new EntityMock()) { Name = "Sample" });

            MenuItems = new DynamicGroupItem("Root");
            MenuItems.AddCommand("Cut","Ctrl+X", CutEntity).Icon = OutlinerItem.EntityIcon;
            MenuItems.AddCommand("Copy","Ctrl+C", CutEntity).Icon = OutlinerItem.EntityIcon;
            MenuItems.AddCommand("Paste","Ctrl+V", CutEntity);
            MenuItems.AddSeparator();
            MenuItems.AddCommand("Rename", CutEntity);
            MenuItems.AddCommand("Duplicate", CutEntity);
            MenuItems.AddCommand("Delete", CutEntity);
            MenuItems.AddSeparator();
            MenuItems.AddCommand("Create Empty","Ctrl+N", CutEntity);
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
            History.ClearForTest();
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
                while(parent != null)
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

            foreach(var item in list)
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

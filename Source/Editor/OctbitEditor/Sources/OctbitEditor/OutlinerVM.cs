using System.Collections.ObjectModel;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Reactive.Bindings;

namespace OctbitEditor
{
    public class OutlinerItem
    {
        internal static BitmapImage FolderIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/folder.png"));
        internal static BitmapImage EntityIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/entity.png"));

        public OutlinerItem(string name = "New Item") { 
            Name.Value = name;
            Icon.Value = EntityIcon;
        }

        public ReactiveProperty<string> Name { get; } = new();
        public ReactivePropertySlim<BitmapImage> Icon { get; } = new();
        public ReactiveProperty<bool> IsExpanded{ get; } = new(true);
        public ReactiveProperty<bool> IsSelected { get; } = new(false);
        public ObservableCollection<OutlinerItem> Children { get; } = new();
    }

    public class OutlinerVM
    {
        public OutlinerVM()
        {
            Children.Add(new OutlinerItem());
            Children.Add(new OutlinerItem());
            Children.Add(new OutlinerItem());

            foreach (var item in Children)
            {
                item.Icon.Value = OutlinerItem.FolderIcon;
            }

            Children[0].Children.Add(new OutlinerItem());
            Children[0].Children.Add(new OutlinerItem());

            Children[0].Children[0].Children.Add(new OutlinerItem());
            Children[0].Children[0].Children.Add(new OutlinerItem());
        }

        public ObservableCollection<OutlinerItem> Children { get; } = new();
    }
}

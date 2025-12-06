using CommonView.Controls;
using System.Collections.ObjectModel;
using System.Windows.Controls;
using System.Windows;

namespace OctbitEditor.Controls.AssetBrowser.Breadcrumb
{
    public interface IAssetBrowserBreadcrumbItem
    {

    }

    public class AssetBrowserBreadcrumb
    {

        public string Path
        {
            get => string.Concat(Items.Select(i=>i switch { AssetBrowserBreadcrumbItem item => item.Name.Value,_ => "/" }));
            set
            {
                Items.Clear();
                foreach (var item in value.Split('/'))
                {
                    Items.Add(new AssetBrowserBreadcrumbItem(item));
                    Items.Add(new AssetBrowserBreadcrumbSeparator());
                }
            }
        }

        public ObservableCollection<object> Items = new();

        public AssetBrowserBreadcrumb()
        {
        }

    }


    public class AssetBrowserBreadcrumbDataSelector : DataTemplateSelector
    {
        static AssetBrowserBreadcrumbDataSelector()
        {

        }

        public override DataTemplate? SelectTemplate(object item, DependencyObject container)
        {
            if (container is not FrameworkElement f) return null;
            if (item is InspectableProperty p)
            {
                return f.FindResource("AssetBrowserBreadcrumbItem") as DataTemplate;
            }
            if (item is InspectableMethod m)
            {
                return f.FindResource("AssetBrowserBreadcrumbSeparator") as DataTemplate;
            }
            return null;
        }

    }
}

using System.Collections.ObjectModel;

namespace OctbitEditor.Controls.AssetBrowser.Breadcrumb
{
    internal class AssetBrowserBreadcrumbSeparator
    {
        public ObservableCollection<string> Items { get; } = new();

        public AssetBrowserBreadcrumbSeparator()
        {

        }

    }
}

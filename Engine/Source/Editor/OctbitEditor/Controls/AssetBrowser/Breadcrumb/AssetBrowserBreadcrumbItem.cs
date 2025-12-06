using Livet;
using Reactive.Bindings;

namespace OctbitEditor.Controls.AssetBrowser.Breadcrumb
{
    internal class AssetBrowserBreadcrumbItem : ViewModel
    {
        public ReadOnlyReactivePropertySlim<string> Name { get; }

        public AssetBrowserBreadcrumbItem(string name)
        {
            Name = new ReactivePropertySlim<string>(name).ToReadOnlyReactivePropertySlim<string>();
        }
    }
}

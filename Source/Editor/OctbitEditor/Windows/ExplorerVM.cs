using Livet;
using OctbitEngine.Asset;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows.Media.Imaging;

namespace OctbitEditor
{
    internal class ExplorerItem
    {
        internal static BitmapImage FolderIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/folder.png"));
        internal static BitmapImage AssetIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/entity.png"));
        public ExplorerItem()
        {
            Name.Value = "";
            Icon.Value = FolderIcon;
            Path = "";
        }
        public ExplorerItem(IAsset asset,bool isAsset = false)
        {
            Name.Value = asset.Name;
            Icon.Value = asset is IAssetFile?AssetIcon : FolderIcon;
            Path = asset.Path;
        }


        public string Path { get; }

        public ReactiveProperty<string> Name { get; } = new();
        public ReactiveProperty<bool> IsSelected { get; } = new();
        public ReactiveProperty<bool> IsExpanded { get; } = new(false);
        public ReactivePropertySlim<BitmapSource> Icon { get; } = new();

        public ObservableCollection<ExplorerItem> Children { get; } = new();
    }

    internal class ExplorerVM : ViewModel
    {
        public ExplorerVM()
        {
            void visit(IAssetFolder folder,ExplorerItem parent)
            {
                foreach (var child in folder.ChildFolders)
                {
                    var item = new ExplorerItem(child);
                    parent.Children.Add(item);
                    visit(child,item);
                }
                foreach(var child in folder.ChildFiles)
                {
                    var item = new ExplorerItem(child);
                    parent.Children.Add(item);
                }
            }

            m_manager = new AssetManager();

            var rootItem = new ExplorerItem(m_manager.RootFolder);
            Children.Add(rootItem);
            visit(m_manager.RootFolder, rootItem);

            SelectedFolder.Subscribe(item =>
            {
                SelectedFolderPath.Value = (item?.Path??"Asset").Replace("\\","/");
            });

        }

        AssetManager m_manager;


        public ObservableCollection<ExplorerItem> Children { get; } = new();
        public ReactiveProperty<ExplorerItem> SelectedFolder { get; } = new();
        public ReactiveProperty<string> SelectedFolderPath { get; } = new("Asset");
    }
}

using Livet;
using OctbitEngine.Asset;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.Drawing;
using System.Windows.Data;
using System.Windows.Media.Imaging;

namespace OctbitEditor.Controls.AssetBrowser
{
    public class AssetBrowserItem : ViewModel
    {
        protected AssetBrowserItem()
        {
            ChildFolders = new CollectionViewSource() { Source = Children };
            ChildFolders.View.Filter = i => i is AssetBrowserFolderItem;
        }

        //public AssetBrowserItem? Parent { get; }
        public ReactiveProperty<bool> IsSelected { get; } = new();
        public ReactiveProperty<bool> IsSelectedInList { get; } = new();
        public ReactiveProperty<bool> IsExpanded { get; } = new(false);
        public ObservableCollection<AssetBrowserItem> Children { get; } = new();
        public CollectionViewSource ChildFolders { get; }
    }
    public class AssetBrowserFolderItem : AssetBrowserItem
    {
        private static BitmapImage DefaultIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/folder.png"));
        public BitmapSource Icon => DefaultIcon;
        public IAssetFolder Folder { get; }

        public AssetBrowserFolderItem(IAssetFolder folder)
        {
            Folder = folder;
        }
    }
    public class AssetBrowserFileItem : AssetBrowserItem
    {
        internal static BitmapImage DefaultIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/icon.ico"));
        public BitmapSource Icon
        {
            get
            {
                if (0 < Children.Count && Children[0] is AssetBrowserAssetItem asset)
                {
                    // アイコン変更の検知が必要
                    return asset.Icon;
                }
                if (File.Assets.Count == 1)
                {
                    // TODO アセット専用アイコンを取得
                    return DefaultIcon;
                }

                return DefaultIcon;
            }
        }
        public string Name
        {
            get => File.Name;
            set
            {
                //File.Name = value;
                throw new NotImplementedException("Fileのリネームは未実装です");
            }
        }

        public IAssetFile File { get; }

        public AssetBrowserFileItem(IAssetFile file)
        {
            File = file;
        }
    }
    public class AssetBrowserAssetItem : AssetBrowserItem
    {
        private static BitmapImage DefaultIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/icon.ico"));
        public BitmapSource Icon => DefaultIcon;
        private IAsset Asset { get; }

        public string Name => Asset.Name;

        public AssetBrowserAssetItem(IAsset asset)
        {
            Asset = asset;
        }
    }

}

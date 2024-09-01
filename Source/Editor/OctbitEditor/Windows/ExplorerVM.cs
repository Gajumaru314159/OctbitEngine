using CommonView.Menu;
using Livet;
using Common;
using OctbitEngine.Asset;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows.Media.Imaging;
using OctbitEngine.Config;
using CommonView;

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

            m_manager = new AssetManager();

            if (Design.IsInDesignMode) return;

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
            var rootItem = new ExplorerItem(m_manager.RootFolder);
            SelectedFolder.Value = rootItem;
            Children.Add(rootItem);
            visit(m_manager.RootFolder, rootItem);

            SelectedItems.CollectionChanged += (sender, e) =>
            {
                if(0 < (e.NewItems?.Count??0))
                {
                    SelectedFolder.Value = (ExplorerItem)(e.NewItems[0]!);
                }
                else
                {
                    SelectedFolder.Value = rootItem;
                }
            };

            SelectedFolder.Subscribe(item =>
            {
                SelectedFolderPath.Value = (item?.Path??"Asset").Replace("\\","/");
            });

            GenerateMenuItems();
        }

        private void GenerateMenuItems()
        {
            {
                var group = MenuItems.AddGroup("_Create");
                group.AddEmptyCommand("Folder","");
                group.AddSeparator();

                // Import専用アセット以外を生成
                group.AddEmptyCommand("Scene");
                group.AddEmptyCommand("Material");

            }
            MenuItems.AddCommand("Show in Explorer", ShowInExplorer);
            MenuItems.AddEmptyCommand("Open");
            MenuItems.AddEmptyCommand("Delete");
            MenuItems.AddEmptyCommand("Dename");
            MenuItems.AddEmptyCommand("Copy Path");
            MenuItems.AddSeparator();
            MenuItems.AddEmptyCommand("Reimport");
            MenuItems.AddSeparator();
            MenuItems.AddEmptyCommand("Show Dependencies");
        }

        private void ShowInExplorer()
        {
            if (SelectedFolder.Value == null) return;
            var actualPath = Path.GetFullPath(Path.Combine(WorkSpace.RootPath, SelectedFolder.Value.Path));
            System.Diagnostics.Process.Start("explorer.exe", actualPath);
        }

        AssetManager m_manager;


        public DynamicGroupItem MenuItems { get; } = new("Root");

        public ObservableCollection<ExplorerItem> Children { get; } = new();
        public ObservableCollection<ExplorerItem> SelectedItems { get; set; } = new();
        public ReactiveProperty<ExplorerItem> SelectedFolder { get; } = new();
        public ReactiveProperty<string> SelectedFolderPath { get; } = new("Asset");
    }
}

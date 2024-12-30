using Common.Generic;
using Common.Log;
using Common.Tree;
using CommonView.Menu;
using Livet;
using OctbitEngine.Asset;
using OctbitEngine.Config;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.IO;
using System.Reactive.Linq;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace OctbitEditor
{

    public enum ListViewType
    {
        List,
        Detail,
        Icon,
    }
    public class AssetBrowserItem : ViewModel
    {
        internal static BitmapImage FolderIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/folder.png"));

        protected AssetBrowserItem()
        {
            ChildFolders = new CollectionViewSource() { Source = Children };
            ChildFolders.View.Filter = i => i is AssetBrowserFolderItem;
        }

        public AssetBrowserItem? Parent { get; set; } // TODO setを排除
        public virtual Brush ItemColorBrush => Brushes.Transparent;
        public virtual BitmapSource Icon => FolderIcon;
        public virtual string Name { get; set; } = "-";
        public virtual bool IsEditable => false;
        public ReactiveProperty<bool> IsSelected { get; } = new();
        public ReactiveProperty<bool> IsSelectedInList { get; } = new();
        public ReactiveProperty<bool> IsExpanded { get; } = new(false);
        public ObservableCollection<AssetBrowserItem> Children { get; } = new();
        public CollectionViewSource ChildFolders { get; }
    }
    public class AssetBrowserFolderItem : AssetBrowserItem
    {
        public override Brush ItemColorBrush => Brushes.Red;
        public override BitmapSource Icon => FolderIcon;
        public override string Name {
            get => Folder.Name;
            set => throw new NotImplementedException("Folderのリネームは未実装です");
        }
        public override bool IsEditable => false;
        public IAssetFolder Folder { get; }

        public AssetBrowserFolderItem(IAssetFolder folder)
        {
            Folder = folder;
        }
    }
    public class AssetBrowserFileItem : AssetBrowserItem
    {
        internal static BitmapImage DefaultIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/icon.ico"));
        public override Brush ItemColorBrush => Brushes.Red;
        public override BitmapSource Icon
        {
            get
            {
                if (0 < Children.Count)
                {
                    // アイコン変更の検知が必要
                    return Children[0].Icon;
                }
                if (File.Assets.Count == 1)
                {
                    // TODO アセット専用アイコンを取得
                    return DefaultIcon;
                }

                return DefaultIcon;
            }
        }
        public override string Name
        {
            get => File.Name;
            set => throw new NotImplementedException("Fileのリネームは未実装です");
        }
        public override bool IsEditable => false;

        public IAssetFile File { get; }

        public AssetBrowserFileItem(IAssetFile file)
        {
            File = file;
        }
    }
    public class AssetBrowserAssetItem : AssetBrowserItem
    {
        internal static BitmapImage AssetIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/icon.ico"));
        public override Brush ItemColorBrush => Brushes.Red;
        public override BitmapSource Icon => AssetIcon;
        public override string Name
        {
            get => Asset.Name;
            set => throw new NotImplementedException("Assetのリネームは未実装です");
        }
        public override bool IsEditable => false;
        private IAsset Asset { get; }

        public AssetBrowserAssetItem(IAsset asset)
        {
            Asset = asset;
        }
    }


    public class AssetBrowserVM : TabBase
    {
        public AssetBrowserVM(IAssetManager assetManager)
            : base("Explorer")
        {
            Title ="Asset Browser";
            AssetManager = assetManager;

            void visit(IAssetFolder folder,AssetBrowserItem parent)
            {
                foreach (var child in folder.ChildFolders)
                {
                    var item = new AssetBrowserFolderItem(child);
                    parent.Children.Add(item);
                    item.Parent = parent;
                    visit(child,item);
                }
                foreach(var child in folder.ChildFiles)
                {
                    var item = new AssetBrowserFileItem(child);
                    parent.Children.Add(item);
                    item.Parent = parent;

                    if (1 < child.Assets.Count)
                    {
                        foreach (var asset in child.Assets)
                        {
                            var assetItem = new AssetBrowserAssetItem(asset);
                            item.Children.Add(assetItem);
                            assetItem.Parent = item;
                        }
                    }
                }
            }
            var rootItem = new AssetBrowserFolderItem(AssetManager.RootFolder);
            rootItem.IsSelected.Value = true;
            rootItem.IsExpanded.Value = true;
            SelectedFolder.Value = rootItem;
            SelectedFolder.Subscribe(_ => RaisePropertyChanged(nameof(SelectionInfo)));

            Children.Add(rootItem);
            visit(AssetManager.RootFolder, rootItem);

            SelectedItems.CollectionChanged += (sender, e) =>
            {
                if(e?.NewItems != null && 0 < e.NewItems.Count)
                {
                    // TODO より安全なアクセスにする
                    if (e.NewItems[0] is AssetBrowserFolderItem folder)
                    {
                        SelectedFolder.Value = folder;
                    }
                    if (e.NewItems[0] is AssetBrowserFileItem file)
                    {
                        if(file.Children.Count == 0)
                        {
                            SelectedFolder.Value = file.Parent!;
                        }
                        else
                        {
                            SelectedFolder.Value = file;
                        }
                    }
                    if (e.NewItems[0] is AssetBrowserAssetItem asset)
                    {
                        SelectedFolder.Value = asset.Parent!;
                    }
                }
                else
                {
                    SelectedFolder.Value = rootItem;
                }

                // TODO もともと選択していたフォルダだけ解除すればよい
                foreach (var i in Children)
                {
                    foreach(var j in i.DepthFirst(i => i.Children))
                    {
                        j.IsSelectedInList.Value = false;
                    }
                }
            };

            SelectedFolder.Subscribe(item =>
            {
                // SelectedFolderPath.Value = (item?.Path??"Asset").Replace("\\","/");
            });

            GenerateMenuItems();

            CreateFolderCommand = new DelegateCommand(CreateFolder);
        }

        private void GenerateMenuItems()
        {
            {
                var group = MenuItems.AddGroup("_Create");
                group.AddCommand("Folder",CreateFolder);
                group.AddSeparator();

                // Import専用アセット以外を生成
                group.AddEmptyCommand("Scene");
                group.AddEmptyCommand("Material");

            }
            MenuItems.AddCommand("Show in Explorer", ShowInExplorer);
            MenuItems.AddCommand("Open",OpenAsset, CanOpenAsset);
            MenuItems.AddCommand("Delete",DeleteAssets);
            MenuItems.AddEmptyCommand("Rename","F2");
            MenuItems.AddEmptyCommand("Copy Path");
            MenuItems.AddSeparator();
            MenuItems.AddEmptyCommand("Reimport");
            MenuItems.AddSeparator();
            MenuItems.AddEmptyCommand("Show Dependencies");
        }

        private void ShowInExplorer()
        {
            AssetBrowserItem? item = SelectedFolder.Value;
            if (item == null) return;
            while (item is not AssetBrowserFolderItem) item = item?.Parent;
            var actualPath = Path.GetFullPath(Path.Combine(WorkSpace.RootPath, ((AssetBrowserFolderItem)item).Folder.PhysicalPath));
            System.Diagnostics.Process.Start("explorer.exe", actualPath);
        }

        private void CreateFolder()
        {
            // var basePath = $"{SelectedFolder.Value.Path}/NewFolder";
            // var createPath = basePath;
            // int index = 1;
            // while (true)
            // {
            //     if (AssetManager.FindFolder(createPath)==null)
            //     {
            //         break;
            //     }
            // 
            //     //string format= "{0}({1})";
            //     string format= "{0}_{1:000}";
            // 
            //     createPath = string.Format(format, basePath, index++);
            // }
            // var newFolder = AssetManager.CreateFolder(createPath);
            // SelectedFolder.Value.Children.Add(new AssetBrowserItem(newFolder));
        }

        private void OpenAsset()
        {
        }

        private void DeleteAssets()
        {
        }

        // Binding Methods
        public void OnSelectionChangedInList()
        {
            RaisePropertyChanged(nameof(SelectionInfo));
        }

        public IAssetManager AssetManager { get; }

        public DynamicGroupItem MenuItems { get; } = new("Root");

        public ObservableCollection<AssetBrowserItem> Children { get; } = new();
        public ObservableCollection<AssetBrowserItem> SelectedItems { get; set; } = new();
        public ObservableCollection<AssetBrowserItem> SelectedItemsInList { get; set; } = new();

        // 選択情報
        public ReactivePropertySlim<AssetBrowserItem> SelectedFolder { get; } = new();
        public ReactivePropertySlim<string> SelectedFolderPath { get; } = new("Asset");
        public ReactivePropertySlim<bool> CanOpenAsset { get; } = new(false);



        public ReactivePropertySlim<double> IconSize { get; } = new(50);

        public IEnumerable<ListViewType> ListViewTypes { get; private set; } = Enum.GetValues<ListViewType>();
        public ReactivePropertySlim<ListViewType> SelectedListViewType { get; } = new(ListViewType.Detail);



        public ICommand CreateFolderCommand { get; }

        public string SelectionInfo => $"{SelectedFolder.Value.Children.Count(i=>i.IsSelectedInList.Value)}/{SelectedFolder.Value.Children.Count} items";
    }
}

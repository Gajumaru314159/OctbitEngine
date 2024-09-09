using Common.Log;
using Common.Tree;
using CommonView.Menu;
using OctbitEngine.Asset;
using OctbitEngine.Config;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.IO;
using System.Reactive.Linq;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using Brush = System.Windows.Media.Brush;
using Brushes = System.Windows.Media.Brushes;

namespace OctbitEditor
{

    public enum ListViewType
    {
        List,
        Detail,
        Icon,
    }
    public class ExplorerItem
    {
        internal static BitmapImage FolderIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/folder.png"));
        internal static BitmapImage AssetIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/entity.png"));
        public ExplorerItem(string name = "Sample")
        {
            Name.Value = name;
            Icon.Value = FolderIcon;
            Path = "";
            IsFolder = true;
        }
        public ExplorerItem(IAssetEntry asset,bool isAsset = false)
        {
            Name.Value = asset.Name;
            Icon.Value = asset is AssetFile?AssetIcon : FolderIcon;
            Path = asset.Path;
            IsFolder = asset is AssetFolder;
        }


        public string Path { get; }

        public bool IsFolder { get; }
        public Brush ItemColorBrush => Brushes.Red;

        public ReactiveProperty<string> Name { get; } = new();
        public ReactiveProperty<bool> IsSelected { get; } = new();
        public ReactiveProperty<bool> IsSelectedInList { get; } = new();
        public ReactiveProperty<bool> IsExpanded { get; } = new(false);
        public ReactivePropertySlim<BitmapSource> Icon { get; } = new();

        public ObservableCollection<ExplorerItem> Children { get; } = new();
    }

    public class ExplorerVM : TabBase
    {
        public ExplorerVM(IAssetManager assetManager)
            : base("Explorer")
        {
            Title ="Asset Browser";
            AssetManager = assetManager;

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
            var rootItem = new ExplorerItem(AssetManager.RootFolder);
            rootItem.IsSelected.Value = true;
            rootItem.IsExpanded.Value = true;
            SelectedFolder.Value = rootItem;
            SelectedFolder.Subscribe(_ => RaisePropertyChanged(nameof(SelectionInfo)));

            Children.Add(rootItem);
            visit(AssetManager.RootFolder, rootItem);

            SelectedItems.CollectionChanged += (sender, e) =>
            {
                if(e!=null && e.NewItems != null && 0 < e.NewItems.Count && e.NewItems[0] is ExplorerItem item && item.IsFolder)
                {
                    SelectedFolder.Value = item;
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
                SelectedFolderPath.Value = (item?.Path??"Asset").Replace("\\","/");
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

        private void CreateFolder()
        {
            var basePath = $"{SelectedFolder.Value.Path}/NewFolder";
            var createPath = basePath;
            int index = 1;
            while (true)
            {
                if (AssetManager.FindFolder(createPath)==null)
                {
                    break;
                }

                //string format= "{0}({1})";
                string format= "{0}_{1:000}";

                createPath = string.Format(format, basePath, index++);
            }
            var newFolder = AssetManager.CreateFolder(createPath);
            SelectedFolder.Value.Children.Add(new ExplorerItem(newFolder));
        }

        private void OpenAsset()
        {
        }

        // Binding Methods
        public void OnSelectionChangedInList()
        {
            RaisePropertyChanged(nameof(SelectionInfo));
        }


        public void OnFileDrop(DragEventArgs e)
        {
            if (e?.Data?.GetDataPresent(DataFormats.FileDrop)??true) return;

            var dropFiles = e.Data.GetData(DataFormats.FileDrop) as string[];

            if (dropFiles == null)
                return;

            Log.Info(string.Join(",", dropFiles));
        }

        public IAssetManager AssetManager { get; }

        public DynamicGroupItem MenuItems { get; } = new("Root");

        public ObservableCollection<ExplorerItem> Children { get; } = new();
        public ObservableCollection<ExplorerItem> SelectedItems { get; set; } = new();

        // 選択情報
        public ReactivePropertySlim<ExplorerItem> SelectedFolder { get; } = new();
        public ReactivePropertySlim<string> SelectedFolderPath { get; } = new("Asset");
        public ReactivePropertySlim<bool> CanOpenAsset { get; } = new(false);



        public ReactivePropertySlim<double> IconSize { get; } = new(50);

        public IEnumerable<ListViewType> ListViewTypes { get; private set; } = Enum.GetValues<ListViewType>();
        public ReactivePropertySlim<ListViewType> SelectedListViewType { get; } = new(ListViewType.Detail);



        public ICommand CreateFolderCommand { get; }

        public string SelectionInfo => $"{SelectedFolder.Value.Children.Count(i=>i.IsSelectedInList.Value)}/{SelectedFolder.Value.Children.Count} items";
    }
}

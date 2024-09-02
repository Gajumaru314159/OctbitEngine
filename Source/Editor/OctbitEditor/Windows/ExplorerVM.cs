using Common.Tree;
using CommonView;
using CommonView.Menu;
using Livet;
using OctbitEngine.Asset;
using OctbitEngine.Config;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.IO;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using Brush = System.Windows.Media.Brush;
using Brushes = System.Windows.Media.Brushes;

namespace OctbitEditor
{
    internal class ExplorerItem
    {
        internal static BitmapImage FolderIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/folder.png"));
        internal static BitmapImage AssetIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/entity.png"));
        public ExplorerItem(string name = "Sample")
        {
            Name.Value = name;
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

        public Brush ItemColorBrush => Brushes.Red;

        public ReactiveProperty<string> Name { get; } = new();
        public ReactiveProperty<bool> IsSelected { get; } = new();
        public ReactiveProperty<bool> IsSelectedInList { get; } = new();
        public ReactiveProperty<bool> IsExpanded { get; } = new(false);
        public ReactivePropertySlim<BitmapSource> Icon { get; } = new();

        public ObservableCollection<ExplorerItem> Children { get; } = new();
    }

    internal class ExplorerVM : ViewModel
    {
        public ExplorerVM()
        {

            if (Design.IsInDesignMode)
            {
                Children.Add(new ExplorerItem("Asset"));
                Children[0].Children.Add(new ExplorerItem("Texture"));
                Children[0].Children.Add(new ExplorerItem("Material"));
                SelectedFolder.Value = Children[0];
                return;
            }

            m_manager = new AssetManager();


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
            rootItem.IsSelected.Value = true;
            SelectedFolder.Value = rootItem;
            SelectedFolder.Subscribe(_ => RaisePropertyChanged(nameof(SelectionInfo)));

            Children.Add(rootItem);
            visit(m_manager.RootFolder, rootItem);

            SelectedItems.CollectionChanged += (sender, e) =>
            {
                if(e!=null && e.NewItems != null && 0 < e.NewItems.Count && e.NewItems[0] is ExplorerItem item)
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

        private void CreateFolder()
        {
            var basePath = $"{SelectedFolder.Value.Path}/NewFolder";
            var createPath = basePath;
            int index = 1;
            while (true)
            {
                if (m_manager.FindFolder(createPath)==null)
                {
                    break;
                }

                //string format= "{0}({1})";
                string format= "{0}_{1:000}";

                createPath = string.Format(format, basePath, index++);
            }
            var newFolder = m_manager.CreateFolder(createPath);
            SelectedFolder.Value.Children.Add(new ExplorerItem(newFolder));
        }

        public void OnSelectionChangedInList()
        {
            RaisePropertyChanged(nameof(SelectionInfo));
        }

        AssetManager? m_manager;


        public DynamicGroupItem MenuItems { get; } = new("Root");

        public ObservableCollection<ExplorerItem> Children { get; } = new();
        public ObservableCollection<ExplorerItem> SelectedItems { get; set; } = new();
        public ReactiveProperty<ExplorerItem> SelectedFolder { get; } = new();
        public ReactiveProperty<string> SelectedFolderPath { get; } = new("Asset");

        public ICommand CreateFolderCommand { get; }

        public string SelectionInfo => $"{SelectedFolder.Value.Children.Count(i=>i.IsSelectedInList.Value)}/{SelectedFolder.Value.Children.Count} items";
    }
}

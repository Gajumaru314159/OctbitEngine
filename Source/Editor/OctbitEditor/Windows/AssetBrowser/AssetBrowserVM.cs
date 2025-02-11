using Common.Generic;
using Common.Linq;
using Common.Log;
using Common.Tree;
using CommonView.Menu;
using Livet;
using OctbitEngine.Asset;
using OctbitEngine.Config;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;
using System.Collections.ObjectModel;
using System.IO;
using System.Reactive.Linq;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace OctbitEditor
{
    public class AssetBrowserTreeDataTemplateSelector : DataTemplateSelector
    {
        public DataTemplate? Shortcut { get; set; }
        public DataTemplate? Folder { get; set; }
        public DataTemplate? File { get; set; }

        public override DataTemplate? SelectTemplate(object item, DependencyObject container)
        {
            return item switch
            {
                AssetBrowserShortcutItem => Shortcut,
                AssetBrowserFolderItem => Folder,
                AssetBrowserFileItem => File,
                _ => null
            };
        }
    }

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
        }

        public AssetBrowserItem? Parent { get; protected init; } // TODO setを排除
        public virtual Brush ItemColorBrush => Brushes.Transparent;
        public virtual BitmapSource Icon => FolderIcon;
        public virtual string Name { get; set; } = "-";
        public virtual bool IsEditable => false;
        public ReactiveProperty<bool> IsSelected { get; } = new();
        public ReactiveProperty<bool> IsSelectedInList { get; } = new();
        public ReactiveProperty<bool> IsExpanded { get; } = new(false);
    }

    /// <summary>
    /// IFolderItemのViewModel
    /// </summary>
    public class AssetBrowserFolderItem : AssetBrowserItem
    {
        public override Brush ItemColorBrush => Brushes.Red;
        public override BitmapSource Icon => FolderIcon;
        public override string Name
        {
            get => Folder.Name;
            set => throw new NotImplementedException("Folderのリネームは未実装です");
        }
        public override bool IsEditable => false;
        public IAssetFolder Folder { get; }
        public ReadOnlyObservableCollection<AssetBrowserItem> Children { get; }

        public CollectionViewSource ChildFolders { get; }

        public AssetBrowserFolderItem(IAssetFolder folder)
            : this(folder, null)
        {
        }

        public AssetBrowserFolderItem(IAssetFolder folder, AssetBrowserItem? parent)
        {
            Parent = parent;
            Folder = folder;
            Children = folder.Children.ToReadOnlyReactiveCollection(GenerateAssetBrowserItem).AddTo(CompositeDisposable);

            ChildFolders = new CollectionViewSource() { Source = Children };
            ChildFolders.View.Filter = i => i is AssetBrowserFolderItem;
        }

        private AssetBrowserItem GenerateAssetBrowserItem(IAssetEntry entry)
        {
            return entry switch
            {
                IAssetFile file => new AssetBrowserFileItem(file, this),
                IAssetFolder folder => new AssetBrowserFolderItem(folder,this),
                _ => throw new System.NotImplementedException()
            };
        }
    }

    /// <summary>
    /// IAssetFileのViewModel
    /// </summary>
    public class AssetBrowserFileItem : AssetBrowserItem
    {
        internal static BitmapImage DefaultIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/icon.ico"));
        public override Brush ItemColorBrush => Brushes.Red;
        public ReadOnlyObservableCollection<AssetBrowserAssetItem> Assets { get; }
        public override BitmapSource Icon
        {
            get
            {
                if (0 < Assets.Count)
                {
                    // アイコン変更の検知が必要
                    return Assets[0].Icon;
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

        public AssetBrowserFileItem(IAssetFile file, AssetBrowserItem parent)
        {
            File = file;
            Parent = parent;
            Assets = file.Assets.ToReadOnlyReactiveCollection(i => new AssetBrowserAssetItem(i, this));
        }
    }

    /// <summary>
    /// IAssetのViewModel
    /// </summary>
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

        public AssetBrowserAssetItem(IAsset asset, AssetBrowserItem parent)
        {
            Parent = parent;
            Asset = asset;
        }
    }



    /// <summary>
    /// 
    /// </summary>
    public class AssetBrowserShortcutItem : AssetBrowserItem
    {
        internal static BitmapImage ShortcutIcon = new BitmapImage(new Uri("pack://application:,,,/OctbitEditor;component/Resources/Icons/Outliner/link.png"));
        public override Brush ItemColorBrush => Brushes.Red;
        public override BitmapSource Icon => ShortcutIcon;
        public override string Name
        {
            get => m_name;
            set => throw new NotImplementedException("Folderのリネームは未実装です");
        }
        public ObservableCollection<AssetBrowserAssetItem> Items { get; } = new();
        public override bool IsEditable => false;

        private string m_name;

        public AssetBrowserShortcutItem(string name)
        {
            m_name = name;
        }
    }






    public partial class AssetBrowserVM : TabBase
    {
        public AssetBrowserVM(IAssetManager assetManager)
            : base("Explorer")
        {
            Title ="Asset Browser";
            AssetManager = assetManager;


            Children.Add(new AssetBrowserShortcutItem("Shortcuts"));

            var rootItem = new AssetBrowserFolderItem(AssetManager.RootFolder);
            Children.Add(rootItem);

            SelectedItemsInTree.ToCollectionChanged().Subscribe(i => {
                m_duaringChange = true;
                if (i.Values?.FirstOrNull() is AssetBrowserFolderItem folder) SelectedFolder.Value=folder;
                m_duaringChange = false;
            });

            SelectedFolder.Value = rootItem;
            SelectedFolder.Subscribe(_ => RaisePropertyChanged(nameof(SelectionInfo)));
            SelectedFolder.Zip(SelectedFolder.Skip(1), (x, y) => new { OldValue = x, NewValue = y })
                .Subscribe(pair =>
                {
                    if(pair.OldValue == pair.NewValue) return;

                    // TODO TreeView空の複数選択を扱う
                    if (m_duaringChange==false) { 
                        SelectedItemsInTree.Clear();
                        SelectedItemsInTree.Add(pair.NewValue);
                    }

                    foreach (var child in pair.OldValue.Children)
                    {
                        child.IsSelectedInList.Value = false;
                    }
                    SelectedFolderPath.Value = (pair.NewValue.Folder.Path);

                    // TODO Redo/Undo時にSelectedItemsInTreeから即時反映されるわけではないのでヒストリの記録方法を変える
                    if (m_executingHistory==false)
                    {
                        m_redoHistory.Clear();
                        m_undoHistory.Push(pair.OldValue);
                        m_executingHistory = false;
                    }
                });

            rootItem.IsExpanded.Value = true;

            GenerateMenuItems();

            MoveUpCommand = new DelegateCommand(MoveUp);
            MoveBackCommand = new DelegateCommand(MoveBack);
            MoveForwardCommand = new DelegateCommand(MoveForward);

            CreateFolderCommand = new DelegateCommand(CreateFolder);
            DeleteCommand = new DelegateCommand(DeleteAssets);
            OpenCommand = new DelegateCommand(OpenAsset);
        }

        public void OnLoaded()
        {
            // Lodedタイミングでないと一度SelectedItemsがリセットされてしまうのでロード後に再選択
            SelectedItemsInTree.Add(Children[1]);
        }


        // Binding Methods
        public void OnSelectionChangedInList()
        {
            RaisePropertyChanged(nameof(SelectionInfo));
        }

        public IAssetManager AssetManager { get; }

        public DynamicGroupItem MenuItems { get; } = new("Root");

        public ObservableCollection<AssetBrowserItem> Children { get; } = new();
        public ObservableCollection<AssetBrowserItem> SelectedItemsInTree { get; set; } = new();
        private bool m_duaringChange = false;

        // 選択情報
        public ReactivePropertySlim<AssetBrowserFolderItem> SelectedFolder { get; } = new(mode: ReactivePropertyMode.RaiseLatestValueOnSubscribe);
        public ReactivePropertySlim<string> SelectedFolderPath { get; } = new("Asset");
        public ReactivePropertySlim<bool> CanOpenAsset { get; } = new(false);


        private IEnumerable<AssetBrowserItem> SelectedItemsInList => SelectedFolder.Value.Children.Where(i => i.IsSelectedInList.Value);

        public ReactivePropertySlim<double> IconSize { get; } = new(50);

        public IEnumerable<ListViewType> ListViewTypes { get; private set; } = Enum.GetValues<ListViewType>();
        public ReactivePropertySlim<ListViewType> SelectedListViewType { get; } = new(ListViewType.Detail);


        // ヒストリ
        private bool m_executingHistory = false;
        private Stack<AssetBrowserFolderItem> m_undoHistory = new();
        private Stack<AssetBrowserFolderItem> m_redoHistory = new();


        public string SelectionInfo => $"{SelectedFolder.Value.Children.Count(i => i.IsSelectedInList.Value)}/{SelectedFolder.Value.Children.Count} items";
    }
}

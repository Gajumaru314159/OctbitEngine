using CommonView.Menu;
using Livet;
using OctbitEditor.Controls.AssetBrowser.Breadcrumb;
using OctbitEngine.Asset;
using OctbitEngine.Config;
using Reactive.Bindings;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace OctbitEditor.Controls.AssetBrowser
{
    public partial class AssetBrowserVM : ViewModel
    {
        private IAssetManager AssetManager { get; }

        public DynamicGroupItem MenuItems { get; } = new("Root");

        // Common
        public IEnumerable<ListViewType> ListViewTypes => Enum.GetValues<ListViewType>();
        public ReactivePropertySlim<ListViewType> SelectedListViewType { get; } = new(ListViewType.Detail);


        // Breadcrumb
        public AssetBrowserBreadcrumb Breadcrumb { get; } = new();

        // Tree

        public ObservableCollection<AssetBrowserItem> Children { get; } = new();

        // List
        public ReactivePropertySlim<AssetBrowserItem> SelectedFolder { get; } = new();
        public ReactivePropertySlim<double> IconSize { get; } = new(50);
        public string SelectionInfo => $"{SelectedFolder.Value.Children.Count(i => i.IsSelectedInList.Value)}/{SelectedFolder.Value.Children.Count} items";





        public AssetBrowserVM(IAssetManager assetManager)
        {
            AssetManager = assetManager;

            void visit(IAssetFolder folder, AssetBrowserItem parent)
            {
                foreach (var child in folder.ChildFolders)
                {
                    var item = new AssetBrowserFolderItem(child);
                    parent.Children.Add(item);
                    visit(child, item);
                }
                foreach (var child in folder.ChildFiles)
                {
                    var item = new AssetBrowserFileItem(child);
                    parent.Children.Add(item);

                    if (1 < child.Assets.Count)
                    {
                        foreach (var asset in child.Assets)
                        {
                            var assetItem = new AssetBrowserAssetItem(asset);
                            item.Children.Add(assetItem);
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


            SelectedFolder.Subscribe(item =>
            {
                // SelectedFolderPath.Value = (item?.Path??"Asset").Replace("\\","/");
            });

            GenerateMenuItems();

        }

    }
}

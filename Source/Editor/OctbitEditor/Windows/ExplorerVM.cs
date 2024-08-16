using Livet;
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
        public ExplorerItem() : this("", "")
        {

        }
        public ExplorerItem(string name,string path,bool isAsset = false)
        {
            Name.Value = name;
            Icon.Value = isAsset?AssetIcon : FolderIcon;
            Path = path;
        }


        public string Path { get; }

        public ReactiveProperty<string> Name { get; } = new();
        public ReactiveProperty<bool> IsSelected { get; } = new();
        public ReactiveProperty<bool> IsExpanded { get; } = new(true);
        public ReactivePropertySlim<BitmapSource> Icon { get; } = new();

        public ObservableCollection<ExplorerItem> Children { get; } = new();
    }

    internal class ExplorerVM : ViewModel
    {
        public ExplorerVM()
        {
            void visit(ExplorerItem parent, string path)
            {
                foreach (var dir in Directory.EnumerateDirectories(path))
                {
                    var item = new ExplorerItem(Path.GetFileName(dir),dir);
                    parent.Children.Add(item);
                    visit(item, dir);
                }
                foreach(var file in Directory.EnumerateFiles(path))
                {
                    var item = new ExplorerItem(Path.GetFileName(file),file,true);
                    parent.Children.Add(item);
                }
            }

            string path = @"D:/My/Productions/C++/OctbitEngine/Asset";


            Children.Add(new ExplorerItem("Assets",path));
            visit(Children[0], path);

            SelectedFolder.Subscribe(item =>
            {
                SelectedFolderPath.Value = (item?.Path??"Asset").Replace("\\","/");
            });

        }


        public ObservableCollection<ExplorerItem> Children { get; } = new();
        public ReactiveProperty<ExplorerItem> SelectedFolder { get; } = new();
        public ReactiveProperty<string> SelectedFolderPath { get; } = new("Asset");
    }
}

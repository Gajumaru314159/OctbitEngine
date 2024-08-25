using OctbitEngine.Config;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static OctbitEngine.Asset.IAssetManager;

namespace OctbitEngine.Asset
{
    public class AssetManager : IAssetManager
    {
        public static char[] InvalidChars = Path.GetInvalidFileNameChars();
        public AssetManager()
        {
            RootFolder = new AssetFolder("Assets");
            LoadAssets();
        }

        private void CheckValidPath(string[] items)
        {
            if (items.Length == 0 || RootFolder.Name != items[0])
            {
                throw new ArgumentException($"パスは{RootFolder.Name}から始まる必要があります。");
            }

            if (items.Any(i => 0 < i.IndexOfAny(InvalidChars)))
            {
                throw new ArgumentException($"パスに使用できない文字が含まれています。{string.Join("", InvalidChars)}");
            }
        }

        public IAssetFolder CreateFolder(string path)
        {
            string[] folderNames = path.Split('/');

            CheckValidPath(folderNames);

            if (FindFolder(path)!=null)
            {
                throw new ArgumentException("フォルダは既に存在します。");
            }

            IAssetFolder? parent= RootFolder;

            foreach (string folderName in folderNames)
            {
                var child = parent.FindFolder(folderName);
                if(child == null)
                {
                    child = new AssetFolder(folderName);
                    child.SetParent(parent);
                }
                parent = child;
            }

            return parent;
        }

        public IAssetFile CreateFile(string path, string type)
        {
            string[] folderNames = path.Split('/');

            CheckValidPath(folderNames);

            throw new NotImplementedException();
        }

        public IAssetFolder? FindFolder(string path)
        {
            string[] folderNames = path.Split('/');

            if (folderNames.Length == 0 || RootFolder.Name != folderNames[0])
            {
                return null;
            }

            IAssetFolder? parent = RootFolder;

            foreach (string folderName in folderNames)
            {
                var child = parent.FindFolder(folderName);
                if (child == null)
                {
                    return null;
                }
                parent = child;
            }

            return parent;
        }


        public IAssetFile? FindFile(string path)
        {
            var folderName = Path.GetDirectoryName(path)??string.Empty;
            var fileName = Path.GetFileName(path)??string.Empty;
            return FindFolder(folderName)?.FindFile(fileName);
        }

        private void LoadAssets()
        {
            void visit(AssetFolder parent, string path)
            {
                foreach (var dir in Directory.EnumerateDirectories(path))
                {
                    var item = new AssetFolder(Path.GetFileName(dir));
                    item.SetParent(parent);
                    parent.Add(item);
                    visit(item, dir);
                }
                foreach (var file in Directory.EnumerateFiles(path))
                {
                    if(Path.GetExtension(file) != ".meta")
                    {
                        if(File.Exists(file + ".meta") == false)
                        {
                            // TODO メタデータを生成して読み込む
                        }
                    }
                }
                foreach (var file in Directory.EnumerateFiles(path))
                {
                    
                    if (Path.GetExtension(file) == ".meta") continue;
                    var item = new AssetFile(Path.GetFileName(file));
                    item.SetParent(parent);
                    parent.Add(item);
                }
            }
            visit((AssetFolder)RootFolder, Path.Combine(WorkSpace.RootPath, "Asset"));
        }

        public IAssetFolder RootFolder { get; private init; }

        public event EventHandler<IAsset>? AssetCreated;

    }
}

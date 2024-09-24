using Common.Generic;
using Common.Linq;
using Common.Log;
using OctbitEngine.Config;

namespace OctbitEngine.Asset
{
    public class AssetManager : IAssetManager
    {
        public static char[] InvalidChars = Path.GetInvalidFileNameChars();
        public static string MetaExtension = ".meta";
        public static string RootFolderName = "Assets";

        private FileSystemWatcher _watcher;

        public static IAssetManager Instance { get; private set; }

        static AssetManager()
        {
            Instance = new AssetManager();
        }

        /// <summary>
        /// 名前に使用できる文字列か
        /// </summary>
        public static bool IsValidName(string name)
        {
            return name.IndexOfAny(InvalidChars) == -1;
        }


        private static List<IAssetImporter> Importers = new List<IAssetImporter>();

        internal AssetManager()
        {
            InitializeImporter();

            RootFolder = new AssetFolder(this, RootFolderName);
            LoadAssets();

            _watcher = new FileSystemWatcher(Path.Combine(WorkSpace.RootPath, RootFolderName));
            _watcher.NotifyFilter = 
                NotifyFilters.FileName | 
                NotifyFilters.DirectoryName | 
                NotifyFilters.LastWrite;

            _watcher.Changed +=OnFileChanged;
            _watcher.Created  +=OnFileChanged;
            _watcher.Deleted  +=OnFileChanged;
            _watcher.Renamed  +=OnFileChanged;
            _watcher.IncludeSubdirectories = true;
            _watcher.EnableRaisingEvents = true;
        }

        private void OnFileChanged(object sender, FileSystemEventArgs e)
        {
            Log.Info($"ファイル変更 : {e.FullPath} ({e.ChangeType})");
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

            IAssetFolder parent= RootFolder;

            foreach (string folderName in folderNames)
            {
                var child = parent?.FindFolder(folderName);
                if(child == null)
                {
                    child = new AssetFolder(this,folderName);
                    child.SetParent(parent!);
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

            IAssetFolder parent = RootFolder;

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
            void visit(IAssetFolder parent, string path)
            {
                // 1. フォルダを読み込む
                foreach (var dir in Directory.EnumerateDirectories(path))
                {
                    var item = new AssetFolder(this,Path.GetFileName(dir));
                    item.SetParent(parent);
                    parent.Add(item);
                    visit(item, dir);
                }
                // 2. ファイルを読み込む
                foreach (var file in Directory.EnumerateFiles(path))
                {
                    if(Path.GetExtension(file) != MetaExtension)
                    {
                        if(File.Exists(file + MetaExtension) == false)
                        {
                            // TODO メタデータを生成して読み込む
                        }
                    }
                }
                // 3. メタデータを読み込む
                foreach (var file in Directory.EnumerateFiles(path))
                {                    
                    if (Path.GetExtension(file) == MetaExtension) continue;
                    var item = new AssetFile(this,Path.GetFileName(file));
                    item.SetParent(parent);
                    parent.Add(item);
                }
            }
            visit(RootFolder, Path.Combine(WorkSpace.RootPath, RootFolder.Name));
        }

        /// <summary>
        /// 指定したパスのファイルをアセットとしてインポートできるか
        /// </summary>
        public bool CanImport(string path)
        {
            return Importers.Any(i => i.CanImport(path));
        }

        public bool Import(string path, IAssetFolder folder)
        {
            foreach(var importer in Importers)
            {
                if (!importer.CanImport(path)) continue;

                var importDest = Path.Combine(folder.PhysicalPath, Path.GetFileName(path));
                if (File.Exists(importDest))
                {
                    // TODO 名前変更
                    continue;
                }

                try
                {
                    File.Copy(path, importDest);
                }
                catch (Exception e)
                {
                    Log.Error($"ファイルのコピーに失敗\n{path} {e}");
                    return false;
                }


                var container = new AssetContainer();
                importer.OnImport(container, path);
                return true;
            }
            return false;
        }

        private void InitializeImporter()
        {
            CoreSystem.Instance.PluginAssemblies
                .Append(typeof(AssetManager).Assembly)
                .SelectMany(i=>i.GetTypes())
                .Where(t => t.IsClass && t.GetInterfaces().Contains(typeof(IAssetImporter)))
                .Select(t => Activator.CreateInstance(t) as IAssetImporter)
                .NotNull()
                .ToList()
                .ForEach(i => Importers.Add(i!));

            foreach (var importer in Importers)
            {
                Log.Info($"AssetImporterを追加 [{importer.GetType().Name}]");
            }
        }

        public IAssetFolder RootFolder { get; private set; }

        public event EventHandler<IAssetEntry>? AssetCreated;

    }
}

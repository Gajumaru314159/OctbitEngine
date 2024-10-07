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


        private static Dictionary<string,IAssetImporter> Importers = new();

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
                List<string>? nometadatas = null;

                // 1. フォルダを読み込む
                foreach (string dir in Directory.EnumerateDirectories(path))
                {
                    var item = new AssetFolder(this,Path.GetFileName(dir));
                    item.SetParent(parent);
                    parent.Add(item);
                    visit(item, dir);
                }
                // 2. ファイルを読み込む
                foreach (string file in Directory.EnumerateFiles(path))
                {                    
                    if (Path.GetExtension(file) == MetaExtension) continue;

                    var metadataPath = file + MetaExtension;

                    AssetFile? item = null;

                    if (File.Exists(metadataPath))
                    {
                        try
                        {
                            string jsonString = File.ReadAllText(metadataPath);
                            var metadata = System.Text.Json.JsonSerializer.Deserialize<AssetMetadata>(jsonString);

                            item = new AssetFile(this, Path.GetFileName(file), metadata.Guid);
                        }
                        catch (Exception e)
                        {
                            Log.Error($"メタデータの読み込みに失敗\n{metadataPath} {e}");
                        }
                    }

                    if(item==null)
                    {
                        if (nometadatas==null) nometadatas = new();
                        nometadatas.Add(file);
                        continue;
                    }

                    item.SetParent(parent);
                    parent.Add(item);
                }

                // 3. メタデータがなかったデータは再処理
                foreach (var file in nometadatas.NotNull())
                {
                    try
                    {
                        var extension = Path.GetExtension(file);
                        var metadataPath = file + MetaExtension;

                        // TODO 不正なメタデータのエラーハンドリングを考える
                        if(File.Exists(metadataPath))
                        {
                            File.Delete(metadataPath);
                        }

                        AssetMetadata metadata = new()
                        {
                            Version = 0,
                            Guid = Guid.NewGuid(),
                        };
                        System.Text.Json.JsonSerializerOptions options = new()
                        {
                            WriteIndented = true,
                        };
                        var bytes = System.Text.Json.JsonSerializer.SerializeToUtf8Bytes(metadata, options);

                        using var stream = File.Create(metadataPath);
                        stream.Write(bytes);

                        // TODO 新規作成アセットの読み込み処理
                    }catch(Exception e)
                    {
                        Log.Error($"メタデータの作成に失敗\n{file} {e}");
                    }
                }
            }
            visit(RootFolder, Path.Combine(WorkSpace.RootPath, RootFolder.Name));
        }

        /// <summary>
        /// 指定したパスのファイルをアセットとしてインポートできるか
        /// </summary>
        public bool CanImport(string path)
        {
            var extension = Path.GetExtension(path);
            if(Importers.TryGetValue(extension,out var importer))
            {
                return importer.CanImport(path);
            }
            return false;
        }

        public bool Import(string path, IAssetFolder folder)
        {
            var extension = Path.GetExtension(path);
            if (Importers.TryGetValue(extension, out var importer))
            {
                if(importer.CanImport(path))return false;

                var importDest = Path.Combine(folder.PhysicalPath, Path.GetFileName(path));
                if (File.Exists(importDest))
                {
                    // TODO 名前変更
                    return false;
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
            CoreSystem.Instance?.PluginAssemblies
                .Append(typeof(AssetManager).Assembly)
                .SelectMany(i=>i.GetTypes())
                .Where(t => t.IsClass && t.GetInterfaces().Contains(typeof(IAssetImporter)))
                .Select(t => Activator.CreateInstance(t) as IAssetImporter)
                .NotNull()
                .ToList()
                .ForEach(importer => importer.EliagebleExtensions.ForEach(extension=>Importers.Add(extension, importer)));

            foreach (var importer in Importers)
            {
                Log.Info($"AssetImporterを追加 [{importer.GetType().Name}]");
            }
        }

        public IAssetFolder RootFolder { get; private set; }

        public event EventHandler<IAssetEntry>? AssetCreated;

    }
}

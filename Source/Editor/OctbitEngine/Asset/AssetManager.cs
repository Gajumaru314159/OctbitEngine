using Common.Linq;
using Common.Log;
using Common.Tree;
using OctbitEngine.Config;
using System.Reflection;
using System.Text.Json;

namespace OctbitEngine.Asset
{
    public class AssetManager : IAssetManager
    {
        public static char[] InvalidChars = Path.GetInvalidFileNameChars();
        public static string MetaExtension = ".meta";
        public static string RootFolderName = "Assets";

        private FileSystemWatcher m_watcher;
        private DefaultAssetImporter m_defaultImporter= new();

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


        private Dictionary<string,IAssetImporter> Importers = new();

        internal AssetManager()
        {
            InitializeImporter();
            InitializeEditor();

            RootFolder = new AssetFolder(RootFolderName);
            LoadAssets();

            m_watcher = new FileSystemWatcher(Path.Combine(WorkSpace.RootPath, RootFolderName));
            m_watcher.NotifyFilter = 
                NotifyFilters.FileName | 
                NotifyFilters.DirectoryName | 
                NotifyFilters.LastWrite;

            m_watcher.Changed +=OnFileChanged;
            m_watcher.Created  +=OnFileChanged;
            m_watcher.Deleted  +=OnFileChanged;
            m_watcher.Renamed  +=OnFileChanged;
            m_watcher.IncludeSubdirectories = true;
            m_watcher.EnableRaisingEvents = true;
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
                    child = new AssetFolder(folderName);
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
            JsonSerializerOptions options = new()
            {
                WriteIndented = true,
                Converters = { 
                    new AssetImporterJsonConverter(
                        CoreSystem.Instance.PluginAssemblies.Append(GetType().Assembly).SelectMany(i=>i.GetTypes()).Where(i=>i.IsAssignableTo(typeof(IAssetImporter))).ToHashSet()
                    )
                }
            };

            void visit(IAssetFolder parent, string path)
            {
                // 1. フォルダを読み込む
                foreach (string dir in Directory.EnumerateDirectories(path))
                {
                    var item = new AssetFolder(Path.GetFileName(dir));
                    item.SetParent(parent);
                    parent.Add(item);
                    visit(item, dir);
                }

                // 2. メタデータがない場合は作成
                foreach (string file in Directory.EnumerateFiles(path))
                {
                    if (Path.GetExtension(file) == MetaExtension) continue;
                    var metadataPath = file + MetaExtension;
                    if (File.Exists(metadataPath))
                    {
                        // メタデータの読み込み
                        try
                        {
                            string jsonString = File.ReadAllText(metadataPath);
                            var t = JsonSerializer.Deserialize<AssetMetadata>(jsonString, options);
                            if(t.Importer is not DefaultAssetImporter)
                            {
                                continue;
                            }
                        }
                        catch (Exception e)
                        {
                            Log.Error($"不正なメタデータを削除\n{metadataPath} {e}");
                            File.Delete(metadataPath);
                        }
                    }

                    // 拡張子からImporterを選択
                    var extension = Path.GetExtension(file);
                    IAssetImporter importer = m_defaultImporter;
                    if (Importers.TryGetValue(extension, out var i))
                    {
                        importer = i;
                    }

                    AssetMetadata metadata = new()
                    {
                        Version = 0,
                        Guid = Guid.NewGuid(),
                        Importer = importer,
                    };

                    var bytes = JsonSerializer.SerializeToUtf8Bytes(metadata, options);

                    using var stream = File.Create(metadataPath);
                    stream.Write(bytes);
                }

                // 3. ファイルを読み込む
                foreach (string file in Directory.EnumerateFiles(path))
                {                    
                    if (Path.GetExtension(file) == MetaExtension) continue;

                    var metadataPath = file + MetaExtension;

                    AssetFile? item = null;

                    // メタデータの読み込み
                    try
                    {
                        string jsonString = File.ReadAllText(metadataPath);
                        var metadata = JsonSerializer.Deserialize<AssetMetadata>(jsonString, options);

                        item = new AssetFile(Path.GetFileName(file), metadata.Guid, metadata.Importer??m_defaultImporter);
                    }
                    catch (Exception e)
                    {
                        Log.Error($"メタデータの読み込みに失敗\n{metadataPath} {e}");
                    }

                    if (item==null)continue;

                    item.SetParent(parent);
                    parent.Add(item);

                    item.Reimport();
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
                if(!importer.CanImport(path))return false;

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

                return true;
            }
            return false;
        }

        private void InitializeImporter()
        {
            CoreSystem.Instance?.PluginAssemblies.Append(typeof(AssetManager).Assembly)
                .SelectMany(i=>i.GetTypes())
                .Where(t => t.IsClass && t.GetInterfaces().Contains(typeof(IAssetImporter)))
                .Select(t => Activator.CreateInstance(t) as IAssetImporter)
                .NotNull()
                .ToList()
                .ForEach(importer => importer.EliagebleExtensions.ForEach(extension=>Importers.Add(extension, importer)));

            var extensions = new Dictionary<IAssetImporter, HashSet<string>>();
            foreach(var (extension,importer) in Importers)
            {
                extensions.TryAdd(importer, new());
                extensions[importer].Add(extension);
            }

            foreach (var (importer,extension) in extensions)
            {
                Log.Info($"AssetImporterを追加 [{importer.GetType().Name}({string.Join(",", extension)})]");
            }
        }


        private void InitializeEditor()
        {
            m_editorMap = CoreSystem.Instance!.PluginAssemblies.Append(typeof(AssetManager).Assembly)
                .SelectMany(i => i.GetTypes())
                .Where(t => t.GetCustomAttribute<AssetEditorAttribute>()!=null)
                .ToDictionary(t => t.GetCustomAttribute<AssetEditorAttribute>()!.Type, t => t);
        }

        public IAssetFolder RootFolder { get; private set; }

        public event EventHandler<IAssetEntry>? AssetCreated;



        public IEnumerable<IAssetFile> AllAssetFile => RootFolder.DepthFirst(i=>i.ChildFolders).SelectMany(i=>i.ChildFiles);



        // 検証
        private Dictionary<Type, Type> m_editorMap=new();
        public Type? FindEditorType(Type? type)
        {
            if(type == null) return null;
            if (m_editorMap.TryGetValue(type, out var editorType)) return editorType;
            return null;
        }


    }
}

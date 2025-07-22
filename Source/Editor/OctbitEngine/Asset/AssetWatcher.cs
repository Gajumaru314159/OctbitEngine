using Common.Log;

namespace OctbitEngine.Asset
{
    public class AssetWatcher
    {
        private FileSystemWatcher m_watcher;
        private readonly string m_root;
        private readonly IAssetManager m_manager;

        public AssetWatcher(IAssetManager manager, string root)
        {
            m_root = root;
            m_manager = manager;

            m_watcher = new FileSystemWatcher(root);
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

            string path = Path.Combine("Assets", Path.GetRelativePath(m_root, e.FullPath)).Replace('\\', '/');

            if (e.ChangeType == WatcherChangeTypes.Created)
            {
                // TODO ファイルとフォルダが存在する場合の対応
                if (Directory.Exists(e.FullPath))
                {
                    if (m_manager.FindFolder(path) == null)
                    {
                        m_manager.CreateFolder(path);
                    }
                }
            }
            if (e.ChangeType == WatcherChangeTypes.Renamed)
            {
                // TODO ファイルとフォルダが存在する場合の対応
                if (Directory.Exists(e.FullPath))
                {
                    if (m_manager.FindFolder(path) is IAssetFolder folder)
                    {
                        folder.Rename(Path.GetFileName(e.FullPath));
                    }
                }
            }

        }

    }
}

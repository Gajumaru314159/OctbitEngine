using Common.Log;
using System.Collections.ObjectModel;

namespace OctbitEngine.Asset
{
    public class AssetFile : AssetEntry, IAssetFile
    {

        public Guid Guid { get; }
        internal AssetFile(string name, Guid guid, IAssetImporter importer, IAssetManager assetManager) : base(name, assetManager)
        {
            Guid =guid;
            Importer = importer;
        }
        public bool Delete()
        {
            // TODO Undoできないことの警告
            // TODO 依存しているファイルがある場合は警告

            try
            {
                File.Delete(PhysicalPath);
                File.Delete(PhysicalPath+OctbitEngine.Asset.AssetManager.MetaExtension);
            }
            catch (Exception e)
            {
                Log.Error(e.Message);
                return false;
            }
            return (Parent as AssetFolder)?.Delete(this)??false;
        }

        public void Reimport()
        {
            var assets = Importer.Import(PhysicalPath);

            lock (m_assets)
            {
                m_assets.Clear();
                foreach (var asset in assets)
                {
                    m_assets.Add(asset);
                }
            }

            // TODO Componentなどに再インポート結果を通知
        }

        public ReadOnlyObservableCollection<IAsset> Assets => new(m_assets);
        private ObservableCollection<IAsset> m_assets = new();
        public IAssetImporter Importer { get; }

    }
}

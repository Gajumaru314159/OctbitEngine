
using OctbitEngine.Config;

namespace OctbitEngine.Asset
{
    public class AssetFile : AssetEntry, IAssetFile
    {

        public Guid Guid { get; }
        internal AssetFile(string name,Guid guid,IAssetImporter importer) : base(name)
        {
            Guid =guid;
            Importer = importer;
        }

        public void Reimport()
        {
            var assets = Importer.Import(PhysicalPath);

            lock (m_assets)
            {
                m_assets = assets;
            }

            // TODO Componentなどに再インポート結果を通知
        }

        public IReadOnlyList<IAsset> Assets => m_assets;
        private IAsset[] m_assets = Array.Empty<IAsset>();
        public IAssetImporter Importer { get; }

    }
}

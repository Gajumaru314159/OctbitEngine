
using OctbitEngine.Config;
using System.Collections.ObjectModel;

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
                m_assets.Clear();
                foreach (var asset in assets) {
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

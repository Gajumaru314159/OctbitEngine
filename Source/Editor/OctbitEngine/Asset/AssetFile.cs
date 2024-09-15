
using OctbitEngine.Config;

namespace OctbitEngine.Asset
{
    public class AssetFile : AssetEntry, IAssetFile
    {
        internal AssetFile(IAssetManager manager,string name) : base(manager,name)
        {
        }

        internal void Serialize()
        {
            // TODO ID管理
            var path = System.IO.Path.Combine(WorkSpace.RootPath,"Binaries", Name);
            using var stream = new FileStream(path, FileMode.Create);
            using var writer = new BinaryWriter(stream);

            lock (this)
            {
                _container.Serialize(writer);
            }
        }

        internal void Reimport()
        {
            // イミュータブルにしたほうが良いかも
            var path = System.IO.Path.Combine(WorkSpace.RootPath, "Assets", $"{Path}.{AssetManager.MetaExtension}");

            var container = new AssetContainer();
            _importer?.OnImport(container, path);

            lock (this)
            {
                _container = container;
            }
        }

        public string AssetType => throw new NotImplementedException();

        public IReadOnlyList<IAsset> Assets => throw new NotImplementedException();

        private AssetContainer _container = new AssetContainer();
        private IAssetImporter? _importer=null;
    }
}

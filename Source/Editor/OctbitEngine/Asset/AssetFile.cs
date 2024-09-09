
namespace OctbitEngine.Asset
{
    public class AssetFile : AssetEntry, IAssetFile
    {
        internal AssetFile(IAssetManager manager,string name) : base(manager,name)
        {
        }

        public string AssetType => throw new NotImplementedException();

        public IReadOnlyList<IAsset> Assets => throw new NotImplementedException();
    }
}

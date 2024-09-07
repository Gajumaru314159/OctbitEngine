namespace OctbitEngine.Asset
{
    public class AssetFile : Asset, IAssetFile
    {
        internal AssetFile(IAssetManager manager,string name) : base(manager,name)
        {
        }

        public string AssetType => throw new NotImplementedException();

    }
}


namespace OctbitEngine.Asset
{
    public abstract class Asset : IAsset
    {
        protected Asset(IAssetFile file,string name,string type)
        {
            File = file;
            Name = name;
            AssetType = type;
        }
        public string AssetType { get; }

        public string Name { get; }

        public IAssetFile File { get; }

        public virtual void Convert(AssetConvertParameter param)
        {

        }

        public virtual void Open()
        {

        }

        public void Serialize(BinaryWriter writer)
        {
            throw new NotImplementedException();
        }
    }
}

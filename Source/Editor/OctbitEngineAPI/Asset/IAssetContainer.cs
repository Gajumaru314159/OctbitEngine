namespace OctbitEngine.Asset
{
    public interface IAssetContainer
    {
        bool Contains(string name);
        bool Add(string name,ISerializable asset);
        bool Map(string from,string to);
    }
}

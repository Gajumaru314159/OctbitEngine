namespace OctbitEngine.Asset
{
    public interface IAssetImporter
    {
        bool CanImport(string path);
        void OnImport(IAssetContainer container,string path);
    }
}

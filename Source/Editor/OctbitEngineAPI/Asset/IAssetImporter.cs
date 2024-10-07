namespace OctbitEngine.Asset
{
    public interface IAssetImporter
    {
        string[] EliagebleExtensions { get; }
        bool CanImport(string path);
        void OnImport(IAssetContainer container,string path);
    }
}

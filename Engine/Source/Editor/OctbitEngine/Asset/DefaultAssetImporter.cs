namespace OctbitEngine.Asset
{
    /// <summary>
    /// インポート可能なAssetImporterが見つからなかった場合に使用されるデフォルトのAssetImporter
    /// </summary>
    internal class DefaultAssetImporter : IAssetImporter
    {
        public string[] EliagebleExtensions => Array.Empty<string>();

        public bool CanImport(string path)
        {
            return false;
        }

        public IAsset[] Import(string path)
        {
            return Array.Empty<IAsset>();
        }
    }
}

namespace OctbitEngine.Asset
{
    public struct AssetMetadata
    {
        public int Version { get; set; }
        public Guid Guid { get; set; }
        public IAssetImporter? Importer { get; set; }

    }

}

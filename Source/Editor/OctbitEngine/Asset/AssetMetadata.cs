namespace OctbitEngine.Asset
{
    public struct AssetMetadata
    {
        public int Version { get; set; }
        public Guid Guid { get; set; }
        public string Importer { get; set; }

        public object? ImportOptions { get; set; }

        // Import Options
        // Dependency
    }

}

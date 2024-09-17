namespace OctbitEngine.Asset
{
    public interface IAssetEntry
    {
        public string Name { get; }

        public string Path { get; }

        public string PhysicalPath { get; }

        public IAssetFolder? Parent { get; }

        public IAssetManager Manager { get; }

        public bool SetParent(IAssetFolder parent);

        public bool Delete();

        public bool Rename(string newName);

        public bool IsAncestorAssetOf(IAssetEntry? asset);
    }
}

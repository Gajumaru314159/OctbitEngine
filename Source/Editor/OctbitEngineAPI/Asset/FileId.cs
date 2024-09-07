namespace OctbitEngine.Asset
{
    public class FileId
    {
        public Guid Guid { get; }
        public IAssetFile? File { get; }

        public FileId(Guid guid)
        {
            Guid = guid;
        }
    }
}

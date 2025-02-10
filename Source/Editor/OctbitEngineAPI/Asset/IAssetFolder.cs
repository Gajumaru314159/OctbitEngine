using System.Collections.ObjectModel;

namespace OctbitEngine.Asset
{
    public interface IAssetFolder : IAssetEntry
    {
        public IAssetFile? FindFile(string name);
        public IAssetFolder? FindFolder(string name);
        public bool Add(IAssetEntry child);
        public bool Import(string path);
        public ReadOnlyObservableCollection<IAssetEntry> Children { get; }
        public IEnumerable<IAssetFolder> ChildFolders{ get; }
        public IEnumerable<IAssetFile> ChildFiles { get; }
    }
}

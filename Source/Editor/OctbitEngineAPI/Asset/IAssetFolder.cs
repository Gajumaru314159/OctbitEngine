using System.Collections.ObjectModel;

namespace OctbitEngine.Asset
{
    public interface IAssetFolder : IAssetEntry
    {
        public IAssetFile? FindFile(string name);
        public IAssetFolder? FindFolder(string name);

        /// <summary>
        /// フォルダを削除する。
        /// 他のアセットから参照されているファイルが含まれている場合は削除できません。
        /// </summary>
        /// <returns>
        /// フォルダの削除に成功したか。
        /// </returns>
        public bool Delete();
        public bool Add(IAssetEntry child);
        public ReadOnlyObservableCollection<IAssetEntry> Children { get; }
        public IEnumerable<IAssetFolder> ChildFolders{ get; }
        public IEnumerable<IAssetFile> ChildFiles { get; }
    }
}

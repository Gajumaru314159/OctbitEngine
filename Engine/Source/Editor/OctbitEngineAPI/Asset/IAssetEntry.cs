namespace OctbitEngine.Asset
{
    public interface IAssetEntry
    {
        public IAssetManager AssetManager { get; }

        /// <summary>
        /// アセット名
        /// 例：Sample.png
        /// </summary>
        public string Name { get; }

        /// <summary>
        /// アセットシステム上のファイルパス
        /// 例：Assets/Sample.png
        /// </summary>
        public string Path { get; }

        /// <summary>
        /// ファイルシステム上のファイルパス
        /// 例：C:/Project/Assets/Sample.png
        /// </summary>
        public string PhysicalPath { get; }

        public IAssetFolder? Parent { get; }

        public bool SetParent(IAssetFolder parent);

        public bool Rename(string newName);

        public bool IsAncestorAssetOf(IAssetEntry? asset);
    }
}

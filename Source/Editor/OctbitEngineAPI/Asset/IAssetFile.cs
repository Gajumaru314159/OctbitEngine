using Common.Linq;

namespace OctbitEngine.Asset
{
    public interface IAssetFile : IAssetEntry
    {
        /// <summary>
        /// AssetFileの所有するプライマリAssetを取得します。
        /// Assets[0]と同じです。
        /// </summary>
        public IAsset? Asset => Assets.FirstOrNull();

        /// <summary>
        /// AssetFileの所有するAssetを取得します。
        /// </summary>
        public IReadOnlyList<IAsset> Assets { get; }

    }
}

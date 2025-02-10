using Common.Linq;
using System.Collections.ObjectModel;

namespace OctbitEngine.Asset
{
    /// <summary>
    /// インポートしたフォルダに対応する
    /// </summary>
    public interface IAssetFile : IAssetEntry
    {
        /// <summary>
        /// AssetFileの所有するプライマリAssetを取得します。
        /// Assets[0]と同じです。
        /// </summary>
        public sealed IAsset? Asset => Assets.FirstOrNull();

        /// <summary>
        /// AssetFileの所有するAssetを取得します。
        /// </summary>
        public ReadOnlyObservableCollection<IAsset> Assets { get; }

        /// <summary>
        /// アセットを再インポートする。
        /// </summary>
        public void Reimport();

        public IAssetImporter Importer { get; }

    }
}

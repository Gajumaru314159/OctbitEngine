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

        /// <summary>
        /// アセットを削除する。
        /// 他のアセットから参照されている場合は削除できません。
        /// </summary>
        /// <returns>
        /// アセットの削除に成功したか。
        /// </returns>
        public bool Delete();

        public IAssetImporter Importer { get; }

    }
}

#nullable enable
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.API
{
    public struct AssetId
    {
        public AssetId() : this(Guid.Empty) { }
        public AssetId(Guid id)
        {
            Id = id;
        }

        public AssetId Generate()
        {
            return new AssetId(Guid.NewGuid());
        }
        public Guid Id { get; }
    }

    public interface IAsset
    {
        // アセットデータを読み込む
        public IAssetData? Load();

        // アセットデータを読み込む
        public T? Load<T>() where T : IAssetData;

        // アセット固有ID
        public AssetId  Id { get; }
        // アセットタイプ (ex:Texture/Model)
        public string   Type { get; }
        // アセット名
        public string   Name { get; }
        // Assetsから始まるパス
        public string   Path { get; }

        // アセット依存関係
        public IReadOnlySet<AssetId> ReferenceDependencies { get; }
        public IReadOnlySet<AssetId> IncludeDependencies { get; }
    }

    public interface IAssetData
    {
    }

    public interface IImportableAssetData : IAssetData
    {
        public string ImportedPath { get; }
        public void Import(string path);
    }

    public interface ITextureData : IImportableAssetData
    {
        public int Width { get; }
        public int Height { get; }
    }
    public interface IModelData : IImportableAssetData
    {
        public string Width { get; }
    }
}

namespace OctbitEngine.Texture
{
    public enum TextureCompression
    {
        Default,
        Normal,
        Masks,
        Grayscale,
    }

    public interface ITexture
    {
        /// <summary>
        /// テクスチャファイルのインポート元のパスです。
        /// この値を変更するだけではテクスチャファイルはインポートされません。
        /// 明示的に Reimport() を呼び出してください。
        /// </summary>
        string Source { get; set; }

        // LOD
        string TextureGroup { get; set; }

        // Compression
        TextureCompression Compression { get; set; }
        bool sRGB { get; set; }
        bool Mipmaps { get; set; }

        void Reimport();
    }
}

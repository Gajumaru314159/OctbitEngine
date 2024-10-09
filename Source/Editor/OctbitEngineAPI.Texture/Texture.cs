using OctbitEngine.Texture;
using System.IO;
using static OctbitEngine.Asset.IAsset;

namespace OctbitEngine.Asset
{
    public class Texture : ITexture, IAsset
    {
        public string Source { get; set; } = string.Empty;

        // LOD
        public string TextureGroup { get; set; } = string.Empty;

        // Compression
        public TextureCompression Compression { get; set; }
        public bool sRGB { get; set; }
        public bool Mipmaps { get; set; }

        public string Name => Path.GetFileName(Source);
        public Texture(string source)
        {
            Source = source;
        }

        public bool Convert(BinaryWriter writer, ConvertOptions options)
        {
            if (!File.Exists(Source)) return false;

            writer.Write(File.ReadAllBytes(Source));

            return true;
        }
    }
}

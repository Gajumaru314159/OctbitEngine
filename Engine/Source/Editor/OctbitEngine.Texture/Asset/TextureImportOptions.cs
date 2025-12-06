using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.Asset
{
    public enum TextureType
    {
        Default,
        NormapMap,
    }

    public enum TextureAlphaSource
    {
        None,
        Alpha,
        Gray,
    }

    public enum TextureResolution
    {
        _32 = 32,
        _64 = 64,
        _128 = 128,
        _256 = 256,
        _512 = 512,
        _1024 = 1024,
        _2048 = 2048,
        _4096 = 4096,
        _8192 = 8192,
    }

    public enum ResizeAlhorithm
    {
        Mitchell,
        Bilinear,
    }

    public class TexturePlatformOptions
    {
        public TextureResolution MaxSize = TextureResolution._2048;
        public ResizeAlhorithm ResizeAlhorithm = ResizeAlhorithm.Mitchell;
    }

    public class TextureImportOptions
    {
        public bool sRGB { get; set; } = true;
        public bool GenerateMipMaps { get; set; } = true;
        public string TextureGroup { get; set; } = "Default";

    }
}

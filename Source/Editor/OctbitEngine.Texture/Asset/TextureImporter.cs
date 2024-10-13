using Common.Attribute;
using Common.String;
using System.IO;

namespace OctbitEngine.Asset
{
    public enum DownScaleMethod
    {
        Linear,
        Nearest,
    }

    public class TextureAssetImporter : IAssetImporter
    {
        [NotInspectable]
        public string[] EliagebleExtensions => new string[] { ".png", ".jpg", ".jpeg" };


        public string TextureGroup { get; set;} = "Default";
        public DownScaleMethod DownScaleMethod { get; set; } = DownScaleMethod.Linear;

        public TextureAssetImporter()
        {
        }

        public bool CanImport(string path)
        {
            return path.MatchExtentions(EliagebleExtensions);
        }

        public IAsset[] Import(string path)
        {
            if(File.Exists(path)==false)return Array.Empty<IAsset>();

            if(Path.GetExtension(path) == ".jpg")
            {
                return [new Texture(path), new Texture(path)];
            }

            return [new Texture(path)];
        }
    }

}

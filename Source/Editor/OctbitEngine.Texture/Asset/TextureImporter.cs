using Common.String;
using System.IO;

namespace OctbitEngine.Asset
{
    public class TextureAssetImporter : IAssetImporter
    {
        public string[] EliagebleExtensions => new string[] { ".png", ".jpg", ".jpeg" };
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

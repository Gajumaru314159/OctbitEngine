using Common.String;

namespace OctbitEngine.Asset
{
    public class TextureAssetImporter : IAssetImporter
    {
        public TextureAssetImporter()
        {
        }

        public bool CanImport(string path)
        {
            return path.MatchExtentions(".png", ".jpg", ".jpeg");
        }
        public bool OnImport(string path)
        {


            return true;
        }
    }

}

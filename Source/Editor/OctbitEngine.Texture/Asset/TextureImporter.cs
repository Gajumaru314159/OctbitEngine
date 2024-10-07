using Common.String;

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
        public void OnImport(IAssetContainer container, string path)
        {
            //var texture = new Texture();
            //container.Add("texture", texture);
            //
            //
            //container.Map("oldTexture", "texture");
        }

    }

}

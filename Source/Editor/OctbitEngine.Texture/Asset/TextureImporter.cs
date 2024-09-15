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
        public void OnImport(IAssetContainer container, string path)
        {
            var texture = new Texture();
            container.Add("texture", texture);


            container.Map("oldTexture", "texture");
        }

    }

}

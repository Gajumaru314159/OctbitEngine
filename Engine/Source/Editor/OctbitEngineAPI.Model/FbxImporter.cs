using Common.String;
using OctbitEngine.Asset;

namespace OctbitEngineAPI.Model
{
    public class FbxImporter : IAssetImporter
    {
        public string[] EliagebleExtensions => [".fbx"];
        public FbxImporter()
        {
        }

        public bool CanImport(string path)
        {
            return path.MatchExtentions(EliagebleExtensions);
        }

        public IAsset[] Import(string path)
        {
            if (File.Exists(path)==false) return Array.Empty<IAsset>();

            return [new Mesh(), new Texture("texture.png"), new Texture("texture.png")];
        }
    }
}

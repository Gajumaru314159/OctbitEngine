using OctbitEngine.Asset;

namespace OctbitEngineAPI.Model
{
    public class Mesh : IMesh
    {
        public string Name => "Mesh";

        public bool Convert(BinaryWriter writer, IAsset.ConvertOptions options)
        {
            throw new NotImplementedException();
        }
    }
}

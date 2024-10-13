using Common.Graphics;
using Common.Math;
using OctbitEngine.Asset;

namespace OctbitEngineAPI.Model
{
    public class Mesh : IMesh
    {
        public string Name => "Mesh";

        public Vector3[] Potisions { get; set; } = Array.Empty<Vector3>();
        public Vector2[] UVs { get; set; } = Array.Empty<Vector2>();
        public Color[] Colors { get; set; } = Array.Empty<Color>();

        public bool Convert(BinaryWriter writer, IAsset.ConvertOptions options)
        {
            throw new NotImplementedException();
        }
    }
}

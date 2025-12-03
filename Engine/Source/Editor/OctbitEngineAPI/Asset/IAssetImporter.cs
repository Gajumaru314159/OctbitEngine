using System.Text.Json.Serialization;

namespace OctbitEngine.Asset
{
    public interface IAssetImporter
    {
        [JsonIgnore]
        string[] EliagebleExtensions { get; }
        bool CanImport(string path);

        IAsset[] Import(string path);
    }
}

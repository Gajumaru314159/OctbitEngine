using System.Text.Json;
using System.Text.Json.Serialization;

namespace OctbitEngine.Asset
{
    [JsonConverter(typeof(FileIdJsonConverter))]
    public class FileId
    {
        public Guid Guid { get; }
        public IAssetFile? File { get; }

        public FileId(Guid guid)
        {
            Guid = guid;
        }
    }

    public class FileIdJsonConverter : JsonConverter<FileId>
    {
        public override FileId Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
        {
            return new FileId(reader.GetGuid());
        }

        public override void Write(Utf8JsonWriter writer, FileId value, JsonSerializerOptions options)
        {
            writer.WriteStringValue(value.Guid);
        }
    }
}

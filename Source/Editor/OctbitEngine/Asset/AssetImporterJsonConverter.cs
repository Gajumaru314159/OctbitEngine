using Common.Linq;
using System.Reflection;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace OctbitEngine.Asset
{
    /// <summary>
    /// 
    /// </summary>
    internal class AssetImporterJsonConverter : JsonConverter<IAssetImporter?>
    {
        private static readonly string TypePropertyName = "Type";
        private static readonly string ValuePropertyName = "Value";

        private HashSet<Type> m_types;
        public override bool CanConvert(Type typeToConvert)
        {
            return typeof(IAssetImporter).IsAssignableTo(typeToConvert);
        }

        public AssetImporterJsonConverter(HashSet<Type> importerTypes)
        {
            m_types = importerTypes;
        }

        public override IAssetImporter? Read(ref Utf8JsonReader reader, Type typeToConvert, JsonSerializerOptions options)
        {
            /// "Type" : "タイプ名"
            /// "Value" : {
            /// }

            if (reader.TokenType != JsonTokenType.StartObject)
                throw new JsonException("開始文字から始まっていない");

            if (!reader.Read() || reader.TokenType != JsonTokenType.PropertyName || reader.GetString() != TypePropertyName)
                throw new JsonException("タイプ指定がない");

            if (!reader.Read() || reader.TokenType != JsonTokenType.String)
                throw new JsonException("タイプ指定が文字列ではない");

            var className = reader.GetString()??string.Empty;
            if (!reader.Read() || reader.GetString() != ValuePropertyName)
                throw new JsonException("Valueが見つからない");

            if (!reader.Read() || reader.TokenType != JsonTokenType.StartObject)
                throw new JsonException("オブジェクトが見つからない");

            IAssetImporter? importer = null;

            try
            {
                var type = m_types.FirstOrNull(i => i.FullName==className);
                if (type==null||!type.IsAssignableTo(typeof(IAssetImporter)))
                {
                    // TODO この実装ではだめらしい
                    reader.Skip();
                    return null;
                }

                importer = (IAssetImporter?)JsonSerializer.Deserialize(ref reader, type, options)!;
            }
            catch
            {
                throw new JsonException();
            }

            if (!reader.Read() || reader.TokenType != JsonTokenType.EndObject)
                throw new JsonException("終了文字で終わっていない");

            return importer;
        }

        public override void Write(Utf8JsonWriter writer, IAssetImporter? value, JsonSerializerOptions options)
        {
            if (value == null)
            {
                writer.WriteNullValue();
                return;
            }
            else
            {
                writer.WriteStartObject();
                writer.WriteString(TypePropertyName, value.GetType().FullName);
                writer.WritePropertyName(ValuePropertyName);
                JsonSerializer.Serialize(writer, value, value.GetType(), options);
                writer.WriteEndObject();
            }
        }

    }
}

using Common.Log;
using System.IO;

namespace OctbitEngine.Asset
{
    public class Texture : ISerializable
    {
        public string Source { get; set; } = string.Empty;

        public Texture()
        {
        }

        public void Deserialize(BinaryReader reader)
        {
            throw new NotImplementedException();
        }

        public void Serialize(BinaryWriter writer)
        {
            if (File.Exists(Source) == false) return;

            try
            {
                using var stream = File.OpenRead(Source);
                var buffer = new byte[4096];
                int bytesRead;
                while ((bytesRead = stream.Read(buffer, 0, buffer.Length)) > 0)
                {
                    writer.Write(buffer, 0, bytesRead);
                }
            }
            catch (Exception e)
            {
                Log.Error(e.Message);
            }
        }
    }
}

using Common.Log;
using OctbitEngine.Texture;
using System.IO;

namespace OctbitEngine.Asset
{
    public class Texture : Asset, ITexture
    {
        public string Source { get; set; } = string.Empty;

        // LOD
        public string TextureGroup { get; set; }

        // Compression
        public TextureCompression Compression { get; set; }
        public bool sRGB { get; set; }
        public bool Mipmaps { get; set; }

        public void Reimport()
        {
            if (System.IO.File.Exists(Source))
            {

                //System.IO.File.Copy(Source, Path.Combine(AssetFile.Directory, Name), true);
            }
        }


        public Texture(IAssetFile file, string name, string type)
             : base(file, name, type)
        {
        }

        public void Deserialize(BinaryReader reader)
        {
            throw new NotImplementedException();
        }

        public override void Serialize(BinaryWriter writer)
        {
            if (System.IO.File.Exists(Source) == false) return;

            try
            {
                using var stream = System.IO.File.OpenRead(Source);
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

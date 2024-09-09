using Common.String;
using System.IO;

namespace OctbitEngine.Asset
{

    public class TextureAsset : Asset
    {
        private string m_source = string.Empty;

        public string Source
        {
            get => m_source;
            set
            {
                if (m_source == value) return;
                m_source = value;


            }
        }

        public TextureAsset(IAssetFile file, string name) : base(file, name, "Texture")
        {
        }



        public override void Convert(AssetConvertParameter param)
        {

        }

        public override void Open()
        {

        }
    }
}

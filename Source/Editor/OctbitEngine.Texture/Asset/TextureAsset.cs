using Common.String;
using System.IO;

namespace OctbitEngine.Asset
{

    public class TextureAsset
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
    }
}

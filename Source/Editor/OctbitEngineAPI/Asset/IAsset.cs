using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.Asset
{
    public struct AssetConvertParameter
    {
        public string Source { get; set; }
        public string Destination { get; set; }
        // Platform
        // Language
    }

    public interface IAsset
    {
        string AssetType { get; }
        string Name { get; }
        IAssetFile File { get; }

        void Convert(AssetConvertParameter param);
        void Open();
    }
}

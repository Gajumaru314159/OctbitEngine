using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.Asset
{
    public interface IAssetFile : IAsset
    {
        public string AssetType { get; }

    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.Asset
{
    internal class AssetFile : Asset,IAssetFile
    {
        internal AssetFile(string name) : base(name)
        {
        }

        public string AssetType => throw new NotImplementedException();

    }
}

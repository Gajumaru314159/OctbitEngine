using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.Asset
{
    public interface IAssetImporter
    {
        bool CanImport(string path);
        void OnImport(string path);
    }
}

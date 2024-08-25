using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.Asset
{
    public class FileId
    {
        public Guid Guid { get; }
        public IAssetFile? File { get; }

        public FileId(Guid guid)
        {
            Guid = guid;
        }
    }
}

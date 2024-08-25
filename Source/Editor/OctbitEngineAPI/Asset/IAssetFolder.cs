using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.Asset
{
    public interface IAssetFolder : IAsset
    {
        public IAssetFile? FindFile(string name);
        public IAssetFolder? FindFolder(string name);
        public bool Remove(IAsset child);
        public IEnumerable<IAsset> Children { get; }
        public IEnumerable<IAssetFolder> ChildFolders{ get; }
        public IEnumerable<IAssetFile> ChildFiles { get; }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Common.Linq;

namespace OctbitEngine.Asset
{
    internal class AssetFolder : Asset,IAssetFolder
    {
        internal AssetFolder(string name) : base(name)
        {
        }

        private List<IAsset> m_children = new List<IAsset>();
        public IEnumerable<IAsset> Children => m_children;
        public IEnumerable<IAssetFolder> ChildFolders => m_children.OfType<IAssetFolder>();
        public IEnumerable<IAssetFile> ChildFiles => m_children.OfType<IAssetFile>();

        public IAssetFile? FindFile(string name)
        {
            foreach(var item in ChildFiles)
            {
                if (item.Name == name)
                {
                    return item;
                }
            }
            return null;
        }

        public IAssetFolder? FindFolder(string name)
        {
            foreach (var item in ChildFolders)
            {
                if (item.Name == name)
                {
                    return item;
                }
            }
            return null;
        }

        public void Add(IAsset child)
        {
            if(child.Parent != null)
            {
                child.Parent.Remove(child);
            }
            m_children.Add(child);
        }

        public bool Remove(IAsset child)
        {
            return m_children.Remove(child);
        }
    }
}

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

        public bool Add(IAsset child)
        {
            if (child.IsAncestorAssetOf(this))
                return false;

            if(child.Parent is AssetFolder folder)
            {
                folder.m_children.Remove(child);
            }

            m_children.Add(child);

            if(child is Asset asset)
            {
                asset.Parent = this;
            }
            
            return true;
        }

    }
}

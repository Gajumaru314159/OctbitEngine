using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Common.Linq;
using Common.Log;

namespace OctbitEngine.Asset
{
    public class AssetFolder : Asset, IAssetFolder
    {
        public AssetFolder(IAssetManager manager,string name) : base(manager,name)
        {
        }

        private List<Asset> m_children = new List<Asset>();
        public IEnumerable<IAsset> Children => m_children;
        public IEnumerable<IAssetFolder> ChildFolders => m_children.OfType<IAssetFolder>();
        public IEnumerable<IAssetFile> ChildFiles => m_children.OfType<IAssetFile>();

        public IAssetFile? FindFile(string name)
        {
            foreach (var item in ChildFiles)
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

            if (child is not Asset cchild)
            {
                return false;
            }

            if (child.Parent is AssetFolder folder)
            {
                folder.m_children.Remove(cchild);
            }

            m_children.Add(cchild);

            cchild.Parent = this;

            return true;
        }

        public bool Import(string path)
        {
            Log.Info($"Import {path}");
            return false;
        }
    }
}

using Common.Log;
using System.Collections.ObjectModel;

namespace OctbitEngine.Asset
{
    public class AssetFolder : AssetEntry, IAssetFolder
    {
        public AssetFolder(string name) : base(name)
        {
        }

        private ObservableCollection<IAssetEntry> m_children = new();
        public ReadOnlyObservableCollection<IAssetEntry> Children => new(m_children);
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

        public bool Add(IAssetEntry child)
        {
            if (child.IsAncestorAssetOf(this))
                return false;

            if (child is not AssetEntry cchild)
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

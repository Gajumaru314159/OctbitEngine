using Common.Tree;

namespace OctbitEngine.Asset
{
    public class AssetEntry : IAssetEntry
    {
        private protected AssetEntry(IAssetManager manager,string name)
        {
            Manager = manager;
            Name = name;
        }

        public string Name { get; private set; } = string.Empty;

        public string Path => string.Join("/", Parent!.Ancestor(i => i.Parent).Reverse().Select(i => i.Name).Append(Name));

        public IAssetFolder? Parent { get; internal set; }

        public IAssetManager Manager { get; }

        public bool SetParent(IAssetFolder parent)
        {
            // 循環チェック
            if (this.IsAncestorAssetOf(parent))
                return false;

            parent.Add(this);

            return true;
        }

        public bool Delete()
        {
            return false;
            //return Parent?.Remove(this)??false;
        }

        public bool Rename(string newName)
        {
            if (AssetManager.IsValidName(newName) == false) return false;
            Name = newName;
            return true;
        }
        public bool IsAncestorAssetOf(IAssetEntry? asset)
        {
            if (asset == null) return false;

            IAssetEntry? p = asset;
            while (p != null)
            {
                if (p == this) return true;
                p = p.Parent;
            }
            return false;
        }


    }
}

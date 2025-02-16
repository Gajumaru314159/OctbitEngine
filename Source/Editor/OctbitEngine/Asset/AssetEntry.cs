using Common.Tree;
using OctbitEngine.Config;

namespace OctbitEngine.Asset
{
    public class AssetEntry : IAssetEntry
    {
        private protected AssetEntry(string name, IAssetManager assetManager)
        {
            AssetManager=assetManager;
            Name = name;
        }
        public IAssetManager AssetManager { get; }

        public string Name { get; private set; } = string.Empty;

        public string Path => string.Join("/", Parent!.Ancestor(i => i.Parent).Reverse().Select(i => i.Name).Append(Name));
        public string PhysicalPath => System.IO.Path.Combine(WorkSpace.RootPath,string.Join("/", Parent!.Ancestor(i => i.Parent).Reverse().Select(i => i.Name).Append(Name)));

        public IAssetFolder? Parent { get; internal set; }

        public bool SetParent(IAssetFolder parent)
        {
            // 循環チェック
            if (this.IsAncestorAssetOf(parent))
                return false;

            parent.Add(this);
            Parent = parent;

            return true;
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

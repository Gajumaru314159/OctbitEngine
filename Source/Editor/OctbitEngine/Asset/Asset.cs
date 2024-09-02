using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using Common.Tree;

namespace OctbitEngine.Asset
{
    internal class Asset : IAsset
    {
        protected Asset(string name)
        {
            Name = name;
        }

        public string Name { get; private set; } = string.Empty;

        public string Path => string.Join("/", Parent!.Ancestor(i => i.Parent).Reverse().Select(i=>i.Name).Append(Name));

        public IAssetFolder? Parent { get; internal set; }

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
            if(IAssetManager.IsValidName(newName) == false) return false;
            Name = newName;
            return true;
        }
        public bool IsAncestorAssetOf(IAsset? asset)
        {
            if (asset == null) return false;

            IAsset? p = asset;
            while (p != null)
            {
                if (p == this) return true;
                p = p.Parent;
            }
            return false;
        }

    }
}

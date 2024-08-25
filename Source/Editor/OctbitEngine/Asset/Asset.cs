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

        public string Name { get; protected set; } = string.Empty;

        public string Path => string.Join("/", Parent!.Ancestor(i => i.Parent).Reverse().Select(i=>i.Name).Append(Name));

        public IAssetFolder? Parent { get; private set; }

        public bool SetParent(IAssetFolder parent)
        {
            var p = parent;
            while (p != null)
            {
                if (p == this) return false;
                p = p.Parent;
            }

            Parent = parent;
            return true;
        }

        public bool Delete()
        {
            return Parent?.Remove(this)??false;
        }

        public bool Rename(string newName)
        {
            if(IAssetManager.IsValidName(newName) == false) return false;
            Name = newName;
            return true;
        }
    }
}

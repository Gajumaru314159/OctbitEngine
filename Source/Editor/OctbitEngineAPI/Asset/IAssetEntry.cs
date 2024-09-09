using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace OctbitEngine.Asset
{
    public interface IAssetEntry
    {
        public string Name { get; }

        public string Path { get; }

        public IAssetFolder? Parent { get; }

        public IAssetManager Manager { get; }

        public bool SetParent(IAssetFolder parent);

        public bool Delete();

        public bool Rename(string newName);

        public bool IsAncestorAssetOf(IAssetEntry? asset);
    }
}

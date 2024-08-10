#nullable enable
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OctbitEngine.API;

namespace OctbitEngine.Core
{
    public abstract class Asset : IAsset
    {
        public AssetId Id { get; }
        public string Type { get; }
        public string Name { get; }
        public string Path { get; }

        virtual public IAssetData? Load()
        {
            throw new NotImplementedException();
        }

        public T? Load<T>() where T : IAssetData
        {
            var data = Load();
            return data is T tdata ? tdata : default;
        }

        public IReadOnlySet<AssetId> ReferenceDependencies => m_referenceDependencies;
        public HashSet<AssetId> m_referenceDependencies = new HashSet<AssetId>();
        public IReadOnlySet<AssetId> IncludeDependencies => m_includeDependencies;
        public HashSet<AssetId> m_includeDependencies = new HashSet<AssetId>();
    }

}

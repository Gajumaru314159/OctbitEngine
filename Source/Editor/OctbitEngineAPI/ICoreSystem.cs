using OctbitEngine.Asset;
using OctbitEngine.Runtime;

namespace OctbitEngine
{
    public interface ICoreSystem
    {
        IAssetManager AssetManager { get; }
        IRuntime Runtime { get; }
        IReadOnlySet<Type> Types { get; }

    }
}

using OctbitEngine.Asset;

namespace OctbitEngine.Runtime
{
    public interface IScene
    {
        public string Name { get; set; }
        public IAssetFile? File { get; }

        public bool IsActive { get; set; }
        public bool IsVisible { get; set; }

        public bool IsActiveInHierarchy { get; }
        public bool IsVisibleInHierarchy { get; }

        public IScene? Parent { get; }
        public IReadOnlyList<IScene> Children { get; }
        public IReadOnlyList<IEntity> Entities { get; }

        public bool SetParent(IScene? parent);
        public bool AddChild(IScene child);


        public IEntity CreateEntity();

    }

}

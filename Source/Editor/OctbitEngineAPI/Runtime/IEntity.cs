using Common.Tree;

namespace OctbitEngine.Runtime
{
    public interface IEntity
    {
        public IScene Scene { get; }

        public string Name { get; set; }

        public bool IsActive { get; set; }
        public bool IsVisible { get; set; }
        public bool IsStatic { get; set; }

        public bool IsActiveInHierarchy => this.AllAncestor(i => i?.Parent, i => i.IsActive);

        public bool IsVisibleInHierarchy => this.AllAncestor(i => i?.Parent, i => i.IsVisible);

        public bool IsStaticInHierarchy => this.AllAncestor(i => i?.Parent, i => i.IsStatic);

        public IEntity? Parent { get; }

        public IReadOnlyList<IEntity> Children { get; }
        public IReadOnlyList<IComponent> Components { get; }

        public ITransformComponent Transform { get; }

        public bool SetParent(IEntity? parent);
        public bool AddChild(IEntity child);

        public IComponent? AddComponent(string type);
        public T? AddComponent<T>() where T : IComponent;
        public IComponent? GetComponent(string type);
        public IComponent[] GetComponents(string type);
        public T? GetComponent<T>() where T : IComponent;
        public T[] GetComponents<T>() where T : IComponent;
    }




    public interface IEntityFolder
    {
        public string Name { get; }

        public IEntityFolder? Parent { get; }

        public IEnumerable<IEntity> Entities { get; }
    }

}

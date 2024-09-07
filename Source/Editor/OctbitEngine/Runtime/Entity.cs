using Common.Tree;

namespace OctbitEngine.Runtime
{
    public sealed class Entity : IEntity
    {

        internal Entity(Guid guid,IRemoteObject remoteObject,IScene scene)
        {
            RemoteObject = remoteObject;
            Scene = scene;
        }

        public IScene Scene { get; }

        public string Name
        {
            get => RemoteObject.GetValue<string>();
            set => RemoteObject.SetValue(value);
        }
        public bool IsActive
        {
            get => RemoteObject.GetValue<bool>();
            set => RemoteObject.SetValue(value);
        }
        public bool IsVisible
        {
            get => RemoteObject.GetValue<bool>();
            set => RemoteObject.SetValue(value);
        }
        public bool IsStatic
        {
            get => RemoteObject.GetValue<bool>();
            set => RemoteObject.SetValue(value);
        }

        public IEntity? Parent => _parent;
        public Entity? _parent;

        public IReadOnlyList<IEntity> Children => m_children;
        private List<Entity> m_children = new();

        public IReadOnlyList<IComponent> Components => m_component;
        private List<IComponent> m_component = new();

        public ITransformComponent Transform => m_transform!;
        private ITransformComponent? m_transform = null;

        public bool AddChild(IEntity _child)
        {
            if (_child is not Entity child) return false;
            if (this.AnyAncestor<IEntity>(i => i.Parent, i => i == child)) return false;
            
            child._parent?.m_children.Remove(child);
            child._parent = this;

            m_children.Add(child);

            return true;
        }
        public bool SetParent(IEntity? parent)
        {
            if (parent is not Entity cparent)
            {
                _parent?.m_children.Remove(this);
                _parent = null;
                return true;
            }

            if (cparent.AnyAncestor(i => i._parent, i => i == this)) return false;
            _parent?.m_children.Remove(this);
            _parent = cparent;
            _parent.m_children.Add(this);
            return true;
        }

        public IComponent? AddComponent(string type)
        {
            throw new NotImplementedException();
        }

        public T? AddComponent<T>() where T : IComponent
        {
            throw new NotImplementedException();
        }

        public IComponent? GetComponent(string type)
        {
            return null;
        }

        public T? GetComponent<T>() where T : IComponent
        {
            throw new NotImplementedException();
        }

        public IComponent[] GetComponents(string type)
        {
            throw new NotImplementedException();
        }

        public T[] GetComponents<T>() where T : IComponent
        {
            throw new NotImplementedException();
        }


        private IRemoteObject RemoteObject { get; init; }
    }
}

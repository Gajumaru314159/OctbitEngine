using Common.Tree;

namespace OctbitEngine.Runtime
{
    internal class Entity : IEntity
    {

        internal Entity(Guid uuid,IClassObject remoteObject,IScene scene)
        {
            m_remoteObject = remoteObject;
            m_scene = scene;
        }

        public IScene Scene => m_scene;
        private IScene m_scene;

        public string Name
        {
            get => m_remoteObject.GetValue("(null)");
            set => m_remoteObject.SetValue(value);
        }
        public bool IsActive
        {
            get => m_remoteObject.GetValue(false);
            set => m_remoteObject.SetValue(value);
        }
        public bool IsVisible
        {
            get => m_remoteObject.GetValue(false);
            set => m_remoteObject.SetValue(value);
        }
        public bool IsStatic
        {
            get => m_remoteObject.GetValue(false);
            set => m_remoteObject.SetValue(value);
        }

        public bool IsActiveInHierarchy => this.AllAncestor<IEntity>(i => i?.Parent, i => i.IsActive);

        public bool IsVisibleInHierarchy => this.AllAncestor<IEntity>(i => i?.Parent, i => i.IsVisible);

        public bool IsStaticInHierarchy => this.AllAncestor<IEntity>(i => i?.Parent, i => i.IsStatic);

        public IEntity? Parent => m_parent;
        private Entity? m_parent;

        public IReadOnlyList<IEntity> Children => m_children;
        private List<Entity> m_children = new();

        public IReadOnlyList<IComponent> Components => m_component;
        private List<Component> m_component = new();

        public ITransformComponent Transform => m_transform!;
        private ITransformComponent? m_transform = null;

        public bool AddChild(IEntity _child)
        {
            if (_child is not Entity child) return false;
            if (this.AnyAncestor<IEntity>(i => i.Parent, i => i == child)) return false;
            
            child.m_parent?.m_children.Remove(child);
            child.m_parent = this;

            m_children.Add(child);

            return true;
        }
        public bool SetParent(IEntity? _parent)
        {
            if (_parent is null)
            {
                m_parent?.m_children.Remove(this);
                m_parent = null;
                return true;
            }
            else if(_parent is Entity parent)
            {
                if (parent.AnyAncestor<IEntity>(i => i.Parent, i => i == this)) return false;
                m_parent?.m_children.Remove(this);
                m_parent = parent;
                parent.m_children.Add(this);
                return true;
            }
            else
            {
                return false;
            }
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
            return m_component.FirstOrDefault(i => i?.RemoteObject.TypeInfo.Name == type,null);
        }

        public T? GetComponent<T>() where T : IComponent
        {
            return m_component.OfType<T?>().FirstOrDefault();
        }

        public IComponent[] GetComponents(string type)
        {
            return m_component.Where(i => i?.RemoteObject.TypeInfo.Name == type).ToArray();
        }

        public T[] GetComponents<T>() where T : IComponent
        {
            return m_component.OfType<T>().ToArray();
        }


        private IClassObject m_remoteObject;
    }
}

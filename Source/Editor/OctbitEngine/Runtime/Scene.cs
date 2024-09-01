using Common.Tree;
using OctbitEngine.Asset;

namespace OctbitEngine.Runtime
{
    internal class Scene : IScene
    {
        internal Scene(IClassObject remoteObject)
        {
            m_remoteObject = remoteObject;
        }

        public string Name
        {
            get => m_remoteObject.GetValue("(null)");
            set => m_remoteObject.SetValue(value);
        }

        public IAssetFile? File => throw new NotImplementedException();

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

        public bool IsActiveInHierarchy => this.AllAncestor<IScene>(i => i?.Parent, i => i.IsActive);

        public bool IsVisibleInHierarchy => this.AllAncestor<IScene>(i => i?.Parent, i => i.IsVisible);

        public IScene? Parent => m_parent;
        private Scene? m_parent;

        public IReadOnlyList<IScene> Children => m_children;
        private List<Scene> m_children = new();

        public IReadOnlyList<IEntity> Entities => m_entities;
        private List<Entity> m_entities = new();


        public bool AddChild(IScene _child)
        {
            if (_child is not Scene child) return false;
            if (this.AnyAncestor<IScene>(i => i.Parent, i => i == child)) return false;

            child.m_parent?.m_children.Remove(child);
            child.m_parent = this;

            m_children.Add(child);

            return true;
        }
        public bool SetParent(IScene? _parent)
        {
            if (_parent is null)
            {
                m_parent?.m_children.Remove(this);
                m_parent = null;
                return true;
            }
            else if (_parent is Scene parent)
            {
                if (parent.AnyAncestor<IScene>(i => i.Parent, i => i == this)) return false;
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


        public IEntity CreateEntity()
        {
            // TODO RemoteObjectに変更
            var obj = new ClassObject(s_typeInfo);
            return new Entity(Guid.NewGuid(),obj,this);
        }


        IClassObject m_remoteObject;

        static ITypeInfo s_typeInfo = new TypeInfo("ob::engine::Entity");
    }
}

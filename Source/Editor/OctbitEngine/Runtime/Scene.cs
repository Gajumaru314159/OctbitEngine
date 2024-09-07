using Common.Tree;
using OctbitEngine.Asset;

namespace OctbitEngine.Runtime
{
    public sealed class Scene : IScene
    {
        internal Scene(IRemoteObject remoteObject)
        {
            m_remoteObject = remoteObject;
        }

        public string Name
        {
            get => m_remoteObject.GetValue<string>();
            set => m_remoteObject.SetValue(value);
        }

        public IAssetFile? File => throw new NotImplementedException();

        public bool IsActive
        {
            get => m_remoteObject.GetValue<bool>();
            set => m_remoteObject.SetValue(value);
        }
        public bool IsVisible
        {
            get => m_remoteObject.GetValue<bool>();
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


        public bool AddChild(IScene child)
        {
            if (child is not Scene cchild) return false;
            if (this.AnyAncestor<IScene>(i => i.Parent, i => i == child)) return false;

            cchild.m_parent?.m_children.Remove(cchild);
            cchild.m_parent = this;

            m_children.Add(cchild);

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
            var obj = new RemoteObject(m_remoteObject.Runtime,s_typeInfo,0);
            return new Entity(Guid.NewGuid(),obj,this);
        }


        private IRemoteObject m_remoteObject;

        static TypeInfo s_typeInfo = new TypeInfo("ob::engine::Entity");
    }
}

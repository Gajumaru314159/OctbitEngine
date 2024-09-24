using Common.Tree;
using OctbitEngine.Asset;

namespace OctbitEngine.Runtime
{
    public sealed class Scene : IScene
    {
        internal Scene(IWorld world,IRemoteObject remoteObject)
        {
            World = world;
            RemoteObject = remoteObject;
        }

        public IWorld World { get; }

        public string Name
        {
            get => RemoteObject.GetValue<string>();
            set => RemoteObject.SetValue(value);
        }

        public IAssetFile? File => throw new NotImplementedException();

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
            int id = RemoteObject.Invoke<int>("createEntity");
            var typeInfo = RemoteObject.Runtime.FindTypeInfo("ob::engine::Entity");
            if(typeInfo==null) throw new Exception("エンティティ型がリフレクション登録されていません");
            return new Entity(Guid.NewGuid(), new RemoteObject(RemoteObject.Runtime, typeInfo, id), this);
        }


        private IRemoteObject RemoteObject { get; }

    }
}

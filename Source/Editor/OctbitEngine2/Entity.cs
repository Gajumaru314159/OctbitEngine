using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.Marshalling;

namespace OctbitEngine2
{
    [Runtime("ob::engine::Entity")]
    public partial class Entity
    {
        public int Id { get; }
        public Scene Scene { get; }
        public IReadOnlyCollection<Component> Components => m_components.AsReadOnly();

        public TransformComponent Transform { get; }

        private List<Component> m_components = new List<Component>();

        internal Entity(Scene scene)
        {
            Scene = scene;

            Transform = AddComponent<TransformComponent>();
        }

        public T AddComponent<T>() where T : Component, new()
        {
            if(typeof(T).GetCustomAttribute<RuntimeAttribute>() is RuntimeAttribute attribute)
            {
                if(!Entity_AddComponent(Id, attribute.Hash))
                {
                    throw new RuntimeException($"{typeof(T).Name}の生成に失敗しました");
                }
            }

            var component = new T() { Entity = this };
            m_components.Add(component);
            return component;
        }

        public bool RemoveComponent<T>(T target) where T : Component
        {
            return m_components.Remove(target);
        }
        public bool RemoveComponent<T>(int index) where T : Component
        {
            int i = m_components.FindIndex(0,index, c => c is T);
            if(i == -1)
            {
                return false;
            }
            m_components.RemoveAt(i);
            return true;
        }

        public T? FindComponent<T>(int index = 0) where T : Component
        {
            return m_components.FindIndex(index, c => c is T) as T;
        }
        public IEnumerable<T> GetComponents<T>() where T : Component
        {
            return m_components.OfType<T>();
        }

        #region Bindings

        [LibraryImport("OctbitEngine")]
        private static partial bool Entity_AddComponent(int id,int hash);

        #endregion

    }
}

namespace OctbitEngine2
{
    [Runtime("ob::engine::World")]
    public partial class World
    {
        public IReadOnlyCollection<Scene> Scenes => m_scenes.AsReadOnly();

        private List<Scene> m_scenes = new List<Scene>();

        public World()
        {
        }
        public Scene CreateScene()
        {
            var scene = new Scene();
            m_scenes.Add(scene);
            return scene;
        }

    }
}

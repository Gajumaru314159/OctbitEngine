namespace OctbitEngine.Runtime
{
    public class World : IWorld
    {
        public World(IRuntime runtime) {
            Runtime = runtime;
        }
        public IScene CreateScene()
        {
            int id = 0;
            // id = Runtime.Send<CreateWorldQuery>();
            var typeInfo = Runtime.FindTypeInfo("ob::engine::Scene");
            if (typeInfo == null) throw new Exception("シーン型がリフレクション登録されていません");
            var scene =  new Scene(this,new RemoteObject(Runtime, typeInfo, id));
            _scene.Add(scene);

            SceneAdded?.Invoke(scene);

            return scene;
        }

        public IRuntime Runtime { get; }
        public IReadOnlyList<IScene> Scenes => _scene;
        private List<IScene> _scene = new();

        public event Action<IScene>? SceneAdded;
        public event Action<IScene>? SceneRemoved;
    }
}

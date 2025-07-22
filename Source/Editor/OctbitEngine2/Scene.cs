using System.Runtime.InteropServices;

namespace OctbitEngine2
{
    public enum SceneStatus
    {
        Loading,
        Building,
        Loaded,
    }


    [Runtime("ob::engine::Scene")]
    public partial class Scene
    {
        public int Id { get; }
        public string Name
        {
            get => Scene_GetName(Id);
            set => Scene_SetName(Id, value);
        }


        public Entity CreateEntity()
        {
            return new Entity(this);
        }

        #region Bindings

        [LibraryImport("OctbitEngine",StringMarshalling = StringMarshalling.Utf8)]
        private static partial string Scene_GetName(int id);

        [LibraryImport("OctbitEngine", StringMarshalling = StringMarshalling.Utf8)]
        private static partial void Scene_SetName(int id,string name);

        #endregion
    }
}

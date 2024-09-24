namespace OctbitEngine.Runtime
{
    public interface IWorld
    {
        IScene CreateScene();

        IReadOnlyList<IScene> Scenes { get; }

        event Action<IScene>? SceneAdded;
        event Action<IScene>? SceneRemoved;
    }
}

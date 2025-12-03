namespace OctbitEngine.Runtime
{
    public interface IComponent
    {
        IRemoteObject RemoteObject { get; }
        IEntity Entity { get; }
    }

}

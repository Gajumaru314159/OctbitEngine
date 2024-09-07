namespace OctbitEngine.Runtime
{
    public class Component : IComponent
    {
        protected Component(Guid guid, IRemoteObject remoteObject, IEntity entity)
        {
            Entity = entity;
            RemoteObject = remoteObject;
        }
        public IEntity Entity { get; }

        public IRemoteObject RemoteObject { get; }
    }

}

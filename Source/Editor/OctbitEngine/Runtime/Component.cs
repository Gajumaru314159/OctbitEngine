namespace OctbitEngine.Runtime
{
    internal class Component : IComponent
    {
        internal Component(Guid uuid, IClassObject remoteObject, Entity entity)
        {
            Entity = entity;
            RemoteObject = remoteObject;
        }
        public IEntity Entity { get; }

        public IClassObject RemoteObject { get; }
    }

}

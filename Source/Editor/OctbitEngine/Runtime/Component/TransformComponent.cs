using Common.Math;

namespace OctbitEngine.Runtime
{
    [RuntimeType("ob::engine::TransformComponent")]
    public class TransformComponent : Component, ITransformComponent
    {
        internal TransformComponent(Guid guid, RemoteObject remoteObject, Entity entity)
            : base(guid, remoteObject, entity)
        {
        }

        public Vector3 Position { get => RemoteObject.GetValue<Vector3>(); set => RemoteObject.SetValue(value); }
        public Rotation Rotation { get => RemoteObject.GetValue<Rotation>(); set => RemoteObject.SetValue(value); }
        public Vector3 Scale { get => RemoteObject.GetValue<Vector3>(); set => RemoteObject.SetValue(value); }
        public Vector3 WorldPosition { get => RemoteObject.GetValue<Vector3>(); set => RemoteObject.SetValue(value); }
        public Rotation WorldRotation { get => RemoteObject.GetValue<Rotation>(); set => RemoteObject.SetValue(value); }
        public Vector3 WorldScale { get => RemoteObject.GetValue<Vector3>(); set => RemoteObject.SetValue(value); }
    }
}

using Common.Math;

namespace OctbitEngine.Runtime
{
    public interface ITransformComponent : IComponent
    {
        public Vector3 Position { get; set; }
        public Rotation Rotation { get; set; }
        public Vector3 Scale { get; set; }
        public Vector3 WorldPosition { get; set; }
        public Rotation WorldRotation { get; set; }
        public Vector3 WorldScale { get; set; }
    }
}

using Common.Math;

namespace OctbitEngine.Runtime
{
    public interface ITransformComponent : IComponent
    {
        public Vector3 Position { get; set; }
        public Vector3 Rotation { get; set; }
        public Vector3 Scale { get; set; }
        public Vector3 WorldPosition { get; set; }
        public Vector3 WorldRotation { get; set; }
        public Vector3 WorldScale { get; set; }
    }
}

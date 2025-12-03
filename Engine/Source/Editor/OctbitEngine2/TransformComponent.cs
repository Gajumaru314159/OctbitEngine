using Common.Math;
using System.Runtime.InteropServices;

namespace OctbitEngine2
{
    [Runtime("ob::engine::TransformComponent")]
    public partial class TransformComponent : Component
    {
        public TransformComponent() { }
        public Vector3 Position
        {
            get => Transform_GetPosition(Entity.Id);
            set => Transform_SetPosition(Entity.Id, value);
        }
        public Rotation Rotation
        {
            get => Transform_GetRotation(Entity.Id);
            set => Transform_SetRotation(Entity.Id, value);
        }
        public Vector3 Scale
        {
            get => Transform_GetScale(Entity.Id);
            set => Transform_SetScale(Entity.Id, value);
        }
        public Vector3 LocalPosition
        {
            get => Transform_GetLocalPosition(Entity.Id);
            set => Transform_SetLocalPosition(Entity.Id, value);
        }
        public Rotation LocalRotation
        {
            get => Transform_GetLocalRotation(Entity.Id);
            set => Transform_SetLocalRotation(Entity.Id, value);
        }
        public Vector3 LocalScale
        {
            get => Transform_GetLocalScale(Entity.Id);
            set => Transform_SetLocalScale(Entity.Id, value);
        }

        public override void Reset() {
            LocalPosition = Vector3.Zero;
            LocalRotation = Rotation.Identity;
            LocalScale = Vector3.One;
        }

        #region Bindings

        [LibraryImport("OctbitEngine")]
        private static partial Vector3 Transform_GetPosition(int id);

        [LibraryImport("OctbitEngine")]
        private static partial void Transform_SetPosition(int id, Vector3 value);

        [LibraryImport("OctbitEngine")]
        private static partial Rotation Transform_GetRotation(int id);

        [LibraryImport("OctbitEngine")]
        private static partial void Transform_SetRotation(int id, Rotation value);

        [LibraryImport("OctbitEngine")]
        private static partial Vector3 Transform_GetScale(int id);

        [LibraryImport("OctbitEngine")]
        private static partial void Transform_SetScale(int id, Vector3 value);


        [LibraryImport("OctbitEngine")]
        private static partial Vector3 Transform_GetLocalPosition(int id);

        [LibraryImport("OctbitEngine")]
        private static partial void Transform_SetLocalPosition(int id, Vector3 value);

        [LibraryImport("OctbitEngine")]
        private static partial Rotation Transform_GetLocalRotation(int id);

        [LibraryImport("OctbitEngine")]
        private static partial void Transform_SetLocalRotation(int id, Rotation value);

        [LibraryImport("OctbitEngine")]
        private static partial Vector3 Transform_GetLocalScale(int id);

        [LibraryImport("OctbitEngine")]
        private static partial void Transform_SetLocalScale(int id, Vector3 value);

        #endregion
    }
}

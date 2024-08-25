using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OctbitEngine.Asset;
using Common.Math;

namespace OctbitEngine.Runtime
{
    public interface IScene
    {
        public string Name { get; set; }
        public IAssetFile? File { get; }
        public IReadOnlyList<IEntity> Entities { get; }
    }

    public interface IComponent
    {
        IEntity Entity { get; }
    }


    public interface ITransformComponent : IComponent
    {
        public Vector3 Position { get; set; }
        public Vector3 Rotation { get; set; }
        public Vector3 Scale { get; set; }
        public Vector3 WorldPosition { get; set; }
        public Vector3 WorldRotation { get; set; }
        public Vector3 WorldScale { get; set; }
    }

    public interface IEntity
    {
        public IScene Scene { get; }

        public string Name { get; set; }

        public bool IsActive { get; set; }
        public bool IsVisible { get; set; }
        public bool IsStatic { get; set; }

        public bool IsActiveInHierarchy { get; }
        public bool IsVisibleInHierarchy { get; }
        public bool IsStaticInHierarchy { get; }

        public IEntity? Parent { get; }

        public IReadOnlyList<IEntity> Children { get; }
        public IReadOnlyList<IComponent> Components { get; }

        public ITransformComponent Transform { get; }

        public bool SetParent(IEntity parent);
        public IComponent? GetComponent(string type);
        public IComponent[] GetComponents(string type);
        public T? GetComponent<T>() where T : IComponent;
        public T[] GetComponents<T>() where T : IComponent;

        public IComponent? AddComponent(string type);
        public T? AddComponent<T>() where T : IComponent;
    }




    public interface IEntityFolder
    {
        public string Name { get; }

        public IEntityFolder? Parent { get; }

        public IEnumerable<IEntity> Entities { get; }
    }

}

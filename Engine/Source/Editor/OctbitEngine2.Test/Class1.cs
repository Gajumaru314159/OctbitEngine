using Common.Math;

namespace OctbitEngine2.Test
{
    public class Class1
    {
        void Test2()
        {
            Test();
        }
        void Test()
        {
            var world = new World();
            var scene = world.CreateScene();
            var entity = scene.CreateEntity();

            var component = entity.AddComponent<ModelComponent>();

            entity.Transform.Position = new Vector3(1, 2, 3);


        }
    }
}

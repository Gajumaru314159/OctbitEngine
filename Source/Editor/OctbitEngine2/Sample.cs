namespace OctbitEngine2
{
    internal class SampleClass
    {
        void Sample2()
        {
            Sample();
        }
        void Sample()
        {
            var scene = new Scene();
            var entity = scene.CreateEntity();


            entity.AddComponent<TransformComponent>();

        }
    }
}

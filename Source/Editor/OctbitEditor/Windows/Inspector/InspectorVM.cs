using Common.Attribute;
using Common.Log;
using Common.Math;
using CommonView.Controls;
using CommonView.Controls.Inspector;
using Reactive.Bindings;

namespace OctbitEditor
{
    public enum TabType
    {
        Inspector,
        History,
    }

    public class ReflectionSub2Test
    {
        public ReflectionSub2Test() { }
        public string Name { get; set; } = "New Entity";
        public int Size { get; set; } = 123;
    }
    public class ReflectionSubTest
    {
        public ReflectionSubTest() { }
        public string Name { get; set; } = "New Entity";
        public int Size{ get; set; } = 123;
        public ReflectionSub2Test Sub2 { get; set; } = new();
    }
    public class ReflectionTest
    {
        public ReflectionTest()
        {

        }

        [DisplayName("メソッド")]
        public void Method()
        {
            Log.Info("Methodを実行");
        }


        [Tooltip("ブール値")]
        public bool BoolValue { get; set; } = true;

        [Space(20)]
        public int IntValue { get; set; } = 123;
        public float FloatValue { get; } = 1.23f;
        public string StringValue { get; set; } = "サンプル";
        public TabType EnumValue { get; set; } = TabType.History;

        public ReflectionSubTest SubTest { get; } = new();
        //public int[] Array { get; set; } = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,};

    }


    public enum TagTest
    {
        Untagged,
        Untagged1,
        Untagged2,
        Untagged3,
        Untagged4,
    }
    public enum LayerTest
    {
        Default
    }

    public class EntityTest
    {
        public EntityTest() { }
        public string Name { get; set; } = "New Entity";
        public bool IsActive { get; set; }
        public bool IsVisible { get; set; }
        public TagTest Tag { get; set; }
        public LayerTest Layer { get; set; }
    }
    public class TransformTest
    {
        public TransformTest() { }
        public Vector3 Translation { get; set; } = Vector3.Zero;
        public Vector3 Rotation { get; set; } = Vector3.Zero;
        public Vector3 Scale { get; set; } = Vector3.One;
    }
    public class ModelTest
    {
        public ModelTest() { }
        public string File { get; set; } = string.Empty;
        public bool IsNavigatable { get; set; } 
        public bool CastShadow { get; set; }
        public bool ReceiveShadow { get; set; }
    }

    public class InspectorVM : TabBase
    {
        public InspectorVM()
            : base("Inspector")
        {
            Inspectables = InspectableReflectionObject.Create(TestObject);

        }

        public IList<Inspectable> Inspectables { get; }

        public ReflectionTest TestObject { get; } = new();
        public EntityTest Entity{ get; } = new();
        public TransformTest Transform { get; } = new();
        public ModelTest Model { get; } = new();


        public ReactivePropertySlim<bool> Expanded { get; } = new(true);
    }
}

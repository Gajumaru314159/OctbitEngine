using Common.Attribute;
using Common.Linq;
using Common.Log;
using Common.Math;
using Common.Tree;
using CommonView;
using CommonView.Controls;
using CommonView.Controls.Inspector;
using OctbitEditor.Windows.Inspector;
using OctbitEngine.Runtime;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.Windows;

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
        [DisplayName("ブール値")]
        public bool BoolValue { get; set; } = true;

        [Space(20)]
        public int IntValue { get; set; } = 123;
        public float FloatValue { get; } = 1.23f;
        public string StringValue { get; set; } = "サンプル";
        public TabType EnumValue { get; set; } = TabType.History;

        public ReflectionSubTest SubTest { get; } = new();
        public int[] Array { get; set; } = [ 1, 2, 3];
        public List<int> List { get; set; } = [ 1, 2, 3];
        public List<ReflectionSub2Test> Vectors { get; set; } = new();

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


    public class InspectableGroup
    {
        public InspectableGroup(string name, IList<Inspectable> inspectables)
        {
            Name = name;
            Inspectables = inspectables;
        }
        public string Name { get; init; } = string.Empty;
        public IList<Inspectable> Inspectables { get; init; } = Array.Empty<Inspectable>();
    }

    public class InspectorVM : TabBase
    {
        public InspectorVM(IRuntime runtime)
            : base("Inspector")
        {
            Entity = new RemoteObject(runtime,runtime.FindTypeInfo("ob::engine::ReflectionTestComponent")!,0);
            Entity.SetValue(new Common.Graphics.Color() { A = 0.8f, R=1.0f, G=0.5f, B=0.0f },"m_color");
            Inspectables = [
                new( nameof(Entity), InspectableRunttimeObject.Create(Entity)),
                new( nameof(Transform), InspectableReflectionObject.Create(Transform)),
                new( nameof(Model), InspectableReflectionObject.Create(Model)),
                new( nameof(TestObject), InspectableReflectionObject.Create(TestObject))
            ];

            Filter.Subscribe(_ => { UpdateFilter(); });
        }

        private void UpdateFilter()
        {
            bool filter(InspectableProperty p)
            {
                return p.Visible = p.Name.Contains(Filter.Value) || string.IsNullOrEmpty(Filter.Value);
            }
            bool visit(InspectableObject obj)
            {
                bool any = false;
                foreach (var inspectable in obj.Inspectables)
                {
                    if (inspectable is InspectableObject o)
                    {
                        if (visit(o))
                        {
                            o.Visible = true;
                        }
                    }
                    if (inspectable is InspectableProperty p)
                    {
                        any |= filter(p);
                    }
                }
                return any;
            }

            foreach (var group in Inspectables)
            {
                foreach(var inspectable in group.Inspectables)
                {
                    if (inspectable is InspectableObject o)
                    {
                        visit(o);
                    }
                    if (inspectable is InspectableProperty p)
                    {
                        filter(p);
                    }
                }
            }

        }


        public Action<ItemMovement> DroppedAction => MoveItem;
        private void MoveItem(ItemMovement itemMovement)
        {
            if (itemMovement.TargetIndex == 0) return;
            if (itemMovement.MovedItem is not InspectableGroup g) return;
            if (g.Name == nameof(Entity)) return;
            itemMovement.MoveItem();
        }

        public ReactivePropertySlim<string> Filter { get; } = new("");
        public ObservableCollection<InspectableGroup> Inspectables { get; }

        public ReflectionTest TestObject { get; } = new();
        public IRemoteObject Entity{ get; }
        public TransformTest Transform { get; } = new();
        public ModelTest Model { get; } = new();


        public ReactivePropertySlim<bool> Expanded { get; } = new(true);
        public ReactivePropertySlim<GridLength> SharedLabelWidth { get; } = new(new GridLength(100));
    }
}

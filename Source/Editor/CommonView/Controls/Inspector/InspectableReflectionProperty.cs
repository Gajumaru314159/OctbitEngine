using Common.Attribute;
using Common.Math;
using System.ComponentModel;
using System.Reflection;

namespace CommonView.Controls.Inspector
{
    public class InspectableReflectionObject : InspectableObject
    {
        public static IList<Inspectable> Create(object obj)
        {
            var result = new List<Inspectable>();

            var type = obj.GetType();

            {
                var properties = type.GetProperties(BindingFlags.Instance | BindingFlags.Public);
                foreach (PropertyInfo p in properties)
                {
                    if (p.PropertyType.IsPrimitive || p.PropertyType.IsEnum || p.PropertyType == typeof(string) || p.PropertyType == typeof(Vector3))
                    {
                        result.Add(new InspectableReflectionProperty(obj, p));
                    }
                    else
                    {
                        var clazz = p.GetValue(obj);
                        if (clazz != null)
                            result.Add(new InspectableReflectionObject(p.Name, clazz));
                    }
                }
            }

            return result;
        }

        private InspectableReflectionObject(string name,object obj)
        {
            Name = name;
            Inspectables = Create(obj);
        }

        public override string Name { get; } = "Name";
        public override IList<Inspectable> Inspectables { get; } = new List<Inspectable>();
    }

    /// <summary>
    /// インスペクタ表示可能なリフレクションプロパティ
    /// </summary>
    internal class InspectableReflectionProperty : InspectableProperty
    {
        public InspectableReflectionProperty(object owner, PropertyInfo propertyInfo)
        {
            Owner = owner;
            PropertyInfo = propertyInfo;

            if(Owner is INotifyPropertyChanged npc)
            {
                // TODO 購読解除
                npc.PropertyChanged += (s, e) =>
                {
                    if (e.PropertyName == Name)
                    {
                        RaisePropertyChanged(nameof(Value));
                    }
                };
            }

            // NOTE プロパティごとに生成する必要はないのでメンバに持たなくてもよい？
            m_tags = propertyInfo.GetCustomAttributes<TagAttribute>().ToDictionary(i=>i.Key,i=>i.Value);

            if (PropertyInfo.ReflectedType?.IsSubclassOf(Owner.GetType())??false)
            {
                throw new ArgumentException("プロパティがオーナーの型に属していません。");
            }
        }

        public object Owner { get; }
        public override string DisplayName => PropertyInfo.Name;
        public override string Name => PropertyInfo.Name;
        public override Type Type => PropertyInfo.PropertyType;
        public override object? Value
        {
            get => PropertyInfo.GetValue(Owner);
            set
            {
                if(value?.GetType() != Type)
                {
                    try
                    {
                        value = Convert.ChangeType(value, Type);
                    }
                    catch
                    {
                        return;
                    }
                }
                if (value == Value) return;

                var oldValue = Value;
                History.History.Record(
                    $"{Name}に値をセット : {value?.ToString()}",
                    () => { PropertyInfo.SetValue(Owner, value); RaisePropertyChanged(); },
                    () => { PropertyInfo.SetValue(Owner, oldValue); RaisePropertyChanged(); }
                );
            }
        }

        public override bool CanRead => PropertyInfo.CanRead;
        public override bool CanWrite => PropertyInfo.CanWrite;
        public override IReadOnlyDictionary<string, string> Tags => m_tags;
        private Dictionary<string, string> m_tags = new();

        public PropertyInfo PropertyInfo { get; }

    }
}

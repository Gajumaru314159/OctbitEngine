using Common.Attribute;
using System.Reflection;

namespace CommonView.Controls.Inspector
{
    internal class InspectableReflectionProperty : IInspectableProperty
    {
        public InspectableReflectionProperty(object owner, PropertyInfo propertyInfo)
        {
            Owner = owner;
            PropertyInfo = propertyInfo;

            // NOTE プロパティごとに生成する必要はないのでメンバに持たなくてもよい？
            m_tags = propertyInfo.GetCustomAttributes<TagAttribute>().ToDictionary(i=>i.Key,i=>i.Value);

            if (PropertyInfo.ReflectedType?.IsSubclassOf(Owner.GetType())??false)
            {
                throw new ArgumentException("プロパティがオーナーの型に属していません。");
            }
        }

        public object Owner { get; }
        public string Name => PropertyInfo.Name;
        public Type Type => PropertyInfo.PropertyType;
        public object? Value
        {
            get => PropertyInfo.GetValue(Owner);
            set
            {
                if (value == Value) return;
                var oldValue = Value;
                History.History.Record(
                    "値をセット",
                    () => PropertyInfo.SetValue(Owner,value),
                    () => PropertyInfo.SetValue(Owner,oldValue)
                );
            }
        }

        public bool CanWrite => PropertyInfo.CanWrite;
        public bool CanRead => PropertyInfo.CanRead;
        public IReadOnlyDictionary<string, string> Tags => m_tags;
        private Dictionary<string, string> m_tags = new();

        public PropertyInfo PropertyInfo { get; }

    }
}

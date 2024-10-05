using Common.Attribute;
using CommonView.Menu;
using Livet.Commands;
using System.Reflection;
using System.Windows.Controls;
using System.Windows.Input;

namespace CommonView.Controls.Inspector
{
    public interface IInspectable
    {

    }

    public class InspectableReflectionObject : IInspectable
    {
        public static IList<IInspectable> Create(object obj)
        {
            var result = new List<IInspectable>();

            var type = obj.GetType();
            {
                var properties = type.GetProperties(BindingFlags.Instance | BindingFlags.Public);
                foreach (PropertyInfo p in properties)
                {
                    result.Add(new InspectableReflectionProperty(obj, p));
                }
            }

            {
                var methods = type.GetMethods(BindingFlags.Instance | BindingFlags.Public | BindingFlags.DeclaredOnly);
                foreach (MethodInfo m in methods)
                {
                    // setter,getter,add,removeなどの特殊なメソッドは無視
                    if (m.IsSpecialName) continue;
                    if (m.GetParameters().Length!=0) continue;

                    result.Add(new InspectableReflectionMethod(obj, m));
                }
            }

            return result;
        }

        public IList<IInspectable> Inspectables { get; } = new List<IInspectable>();
    }


    public class InspectableReflectionMethod : IInspectable
    {
        private MethodInfo m_methodInfo;
        public InspectableReflectionMethod(object owner, MethodInfo methodInfo)
        {
            m_methodInfo = methodInfo;
            Command = new DelegateCommand(() => { m_methodInfo.Invoke(owner, []); });
        }
        public string Name => m_methodInfo.Name;
        public ICommand Command { get; }
    }

    /// <summary>
    /// インスペクタ表示可能なリフレクションプロパティ
    /// </summary>
    internal class InspectableReflectionProperty : IInspectableProperty, IInspectable
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

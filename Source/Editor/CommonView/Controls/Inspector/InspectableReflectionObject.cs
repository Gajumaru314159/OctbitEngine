using System.Reflection;
using Common.Attribute;
using Common.Math;

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
                    if (p.GetCustomAttribute<NotInspectableAttribute>()!=null) continue;

                    // TODO 判定方法を厳密にする
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

            {
                var methods = type.GetMethods(BindingFlags.Instance | BindingFlags.Public | BindingFlags.DeclaredOnly);
                foreach (MethodInfo p in methods)
                {
                    if (p.IsSpecialName) continue;
                    if (p.GetParameters().Length!=0) continue;
                    result.Add(new InspectableReflectionMethod(obj, p));
                }
            }

            return result;
        }

        private InspectableReflectionObject(string name, object obj)
        {
            Name = name;
            Inspectables = Create(obj);
        }

        public override string Name { get; } = "-";
        public override IList<Inspectable> Inspectables { get; } = new List<Inspectable>();
    }
}

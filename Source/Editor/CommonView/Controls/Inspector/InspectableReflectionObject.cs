using Common.Attribute;
using Common.Math;
using System.Collections;
using System.Collections.ObjectModel;
using System.Reflection;

namespace CommonView.Controls.Inspector
{
    public class InspectableReflectionObject : InspectableObject
    {
        public static ObservableCollection<Inspectable> Create(object obj)
        {
            var result = new ObservableCollection<Inspectable>();

            var type = obj.GetType();

            {
                var properties = type.GetProperties(BindingFlags.Instance | BindingFlags.Public);
                foreach (PropertyInfo p in properties)
                {
                    if (p.GetCustomAttribute<NotInspectableAttribute>()!=null) continue;

                    // TODO 判定方法を厳密にする
                    bool hasEditor =
                        p.PropertyType == typeof(bool) ||
                        p.PropertyType == typeof(int) ||
                        p.PropertyType == typeof(float) ||
                        p.PropertyType == typeof(string) ||
                        p.PropertyType == typeof(Vector3) ||
                        p.PropertyType.IsEnum;

                    if (hasEditor)
                    {
                        result.Add(new InspectableReflectionProperty(obj, p));
                    } else if(p.PropertyType.GetInterfaces().Contains(typeof(IList)))
                    {
                        result.Add(new InspectableReflectionList(obj, p));
                    } else if (p.PropertyType.IsClass)
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
        public override ObservableCollection<Inspectable> Inspectables { get; } = new();
    }
}

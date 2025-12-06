using Common.Attribute;
using CommonView.Controls.Inspector;
using CommonView.Controls.Inspector.Reflection;
using Livet;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;

namespace CommonView.Controls
{
    public static class InspectableUtility
    {
        public static T FirstElement<T>(this IEnumerable<InspectableProperty> properties)
        {
            return (T)(properties.First().Value!);
        }
        public static bool AreMulti<T,U>(this IEnumerable<InspectableProperty> properties, Func<T, U> selector)
        {
            return true;
        }
        public static void ApplyValue<T>(this IEnumerable<InspectableProperty> properties, T value)
        {
            foreach (var p in properties)
            {
                p.Value = value;
            }
        }



        private static Dictionary<Type, Type> s_editorMap = new();
        private static bool RegisterEditor(Type type, Type editor)
        {
            return s_editorMap.TryAdd(type, editor);
        }

        
        public static List<Editor> Create(object[] obj)
        {
            var results = new List<Editor>();

            if (0 < obj.Length)
            {

                var target = obj[0];
                var type = target.GetType();

                var objs = obj.Where(i => i.GetType() == type).ToArray();


                {
                    var properties = type.GetProperties(BindingFlags.Instance | BindingFlags.Public);
                    foreach (PropertyInfo p in properties)
                    {
                        if (p.GetCustomAttribute<NotInspectableAttribute>()!=null) continue;

                        var items = objs.Select(i => new InspectableReflectionProperty(obj, p)).ToArray();

                        if (s_editorMap.TryGetValue(p.PropertyType, out Type? editorType))
                        {
                            if (Activator.CreateInstance(editorType, [items]) is Editor vm)
                            {
                                results.Add(vm);
                            }
                        }
                        else if (p.PropertyType.IsEnum)
                        {
                            results.Add(new EnumEditor(items));
                        }
                    }
                }
            }

            return results;
        }
    }
}

using System.Reflection;

namespace OctbitEngine.Runtime
{
    [AttributeUsage(AttributeTargets.Class)]
    public class RuntimeTypeAttribute : Attribute
    {
        public static bool TryGet<T>(out string name)
        {
            var type = typeof(T);
            var attribute = type.GetCustomAttribute<RuntimeTypeAttribute>();
            if (attribute == null)
            {
                name = string.Empty;
                return false;
            }
            name = attribute.Name;
            return true;
        }

        public RuntimeTypeAttribute(string name)
        {
            Name = name;
        }
        public string Name { get; private init; }
    }
}

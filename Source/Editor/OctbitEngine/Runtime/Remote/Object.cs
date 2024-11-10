using Common.ComponentModel;
using System.Runtime.CompilerServices;

namespace OctbitEngine.Runtime
{

    public sealed class UserTypeObject : NotificationObject, IUserTypeObject
    {
        public IReadOnlyDictionary<string, object> Properties => m_properties;
        private Dictionary<string, object> m_properties = new();
        public ITypeInfo TypeInfo { get; }


        internal UserTypeObject(ITypeInfo typeInfo)
        {
            TypeInfo = typeInfo;
            InitializeProperties();
        }

        private void InitializeProperties()
        {
            foreach (var (name,property) in TypeInfo.Properties)
            {
                object? value = null;
                
                if (property.TypeInfo.TryGetBuiltinType(out var type))
                {
                    // Builtin
                    if (type == typeof(string))
                    {
                        value = "";
                    }
                    else
                    {
                        value = Activator.CreateInstance(type!);
                    }
                }
                else if (property.TypeInfo.IsEnum)
                {
                    // Enum
                    value = new int();
                }
                else
                {
                    // Object
                    value = new UserTypeObject(property.TypeInfo);
                }
                if (value is null) continue;
                m_properties[property.Name] = value;
            }
        }


        public object GetValue([CallerMemberName] string? name = null)
        {
            return m_properties[name??string.Empty];
        }

        public T GetValue<T>([CallerMemberName] string? name = null) where T : notnull
        {
            return (T)GetValue(name);
        }

        public bool SetValue(object value, [CallerMemberName] string? name = null)
        {
            if (m_properties.TryGetValue(name ?? string.Empty, out var v))
            {
                if (v.GetType() == value.GetType())
                {
                    m_properties[name ?? string.Empty] = value;
                    return true;
                }
            }
            return false;
        }

        public bool SetValue<T>(T value, [CallerMemberName] string? name = null) where T : notnull
        {
            if (m_properties.TryGetValue(name ?? string.Empty, out var v))
            {
                if (v.GetType() == value.GetType())
                {
                    m_properties[name ?? string.Empty] = value;
                    return true;
                }
            }
            return false;
        }

    }
}

using Common.ComponentModel;
using System.Runtime.CompilerServices;

namespace OctbitEngine.Runtime
{
    public sealed class PrimitiveObject : NotificationObject, IPrimitiveObject
    {
        internal PrimitiveObject(ITypeInfo typeInfo, Type type)
        {
            Type = type;
            TypeInfo = typeInfo;
        }

        public Type Type { get; }

        public ITypeInfo TypeInfo { get; }

        public T GetValue<T>(T fallback) where T : notnull
        {
            if (m_value is not T value) return fallback;
            return value;
        }

        public object GetValue(object fallback)
        {
            return m_value ?? fallback;
        }

        public bool SetValue<T>(T value) where T : notnull
        {
            throw new NotImplementedException();
        }

        public bool SetValue(object value)
        {
            throw new NotImplementedException();
        }

        private object? m_value = null;

    }


    public sealed class ClassObject : NotificationObject, IClassObject
    {
        internal ClassObject(ITypeInfo typeInfo)
        {
            TypeInfo = typeInfo;
        }

        public IReadOnlyDictionary<string, object> Properties => m_properties;
        private Dictionary<string, object> m_properties = new();

        public ITypeInfo TypeInfo { get; }


        public object GetValue(object fallback, [CallerMemberName] string? name = null)
        {
            if (m_properties.TryGetValue(name ?? string.Empty, out var value))
            {
                return value;
            }
            return fallback;
        }

        public T GetValue<T>(T fallback, [CallerMemberName] string? name = null) where T : notnull
        {
            if (m_properties.TryGetValue(name ?? string.Empty, out var value))
            {
                if (value is T result)
                {
                    return result;
                }
            }
            return fallback;
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

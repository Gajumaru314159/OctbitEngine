namespace OctbitEngine.Runtime
{
    public class TagInfo : ITagInfo
    {
        internal TagInfo()
        {
        }
        public IDictionary<string, string> Tags { get; } = new Dictionary<string, string>();
    }

    public sealed class EnumElementInfo : TagInfo, IEnumElementInfo
    {
        internal EnumElementInfo(string name, int index, int value)
        {
            Name = name;
            Index = index;
            Value = value;
        }
        public string Name { get; }
        public int Index { get; }
        public int Value { get; }
    }

    public sealed class ArgumentInfo : TagInfo, IArgumentInfo
    {
        internal ArgumentInfo(ITypeInfo type, string name)
        {
            TypeInfo = type;
            Name = name;
        }
        public ITypeInfo TypeInfo { get; }
        public string Name { get; }
    }

    public sealed class ConstructorInfo : TagInfo,IConstructorInfo
    {
        internal ConstructorInfo(IReadOnlyList<IArgumentInfo> arguments)
        {
            Arguments = arguments;
        }
        public IReadOnlyList<IArgumentInfo> Arguments { get; }
        public object? Invoke(params object?[] args)
        {
            throw new NotImplementedException();
        }
    }

    public sealed class PropertyInfo : TagInfo,IPropertyInfo
    {
        internal PropertyInfo(ITypeInfo typeInfo, string name, bool canRead, bool canWrite)
        {
            TypeInfo = typeInfo;
            Name = name;
            CanRead = canRead;
            CanWrite = canWrite;
        }
        public ITypeInfo TypeInfo { get; }
        public string Name { get; }
        public bool CanRead { get; }
        public bool CanWrite { get; }
        public object? GetValue(object obj)
        {
            throw new NotImplementedException();
        }
        public bool SetValue(object obj, object? value)
        {
            throw new NotImplementedException();
        }
    }

    public sealed class MethodInfo : TagInfo,IMethodInfo
    {
        internal MethodInfo(string name, ITypeInfo returnType, IReadOnlyList<IArgumentInfo> arguments)
        {
            Name = name;
            ReturnType = returnType;
            Arguments = arguments;
        }
        public string Name { get; }
        public ITypeInfo ReturnType { get; }
        public IReadOnlyList<IArgumentInfo> Arguments { get; }
        public object? Invoke(params object?[] args)
        {
            throw new NotImplementedException();
        }
    }

    public sealed class TypeInfo : TagInfo,ITypeInfo
    {


        internal TypeInfo(string name)
        {
            Name = name;
            ShortName = name.Split("::").LastOrDefault()??string.Empty;
            IsEnum = false;
        }
        public string Name { get; set; }
        public string ShortName { get; set; }
        public ISet<ITypeInfo> Bases => m_bases;
        public IReadOnlyList<IConstructorInfo> Constructors => m_constructors;
        public IReadOnlyDictionary<string, IPropertyInfo> Properties => m_properties;
        public IReadOnlyDictionary<string, IMethodInfo> Methods => m_methods;
        public IReadOnlyList<string> PropertyOrder => m_propertyOrder;
        public IReadOnlyList<string> MethodOrder => m_methodOrder;
        public bool IsEnum { get; }
        public bool IsBaseClassOf(ITypeInfo type)
        {
            throw new NotImplementedException();
        }
        public bool IsSuperClassOf(ITypeInfo type)
        {
            throw new NotImplementedException();
        }
        public IConstructorInfo? FindConstructor(params ITypeInfo[] args)
        {
            throw new NotImplementedException();
        }
        public IPropertyInfo? FindProperty(string name)
        {
            throw new NotImplementedException();
        }
        public IMethodInfo? FindMethod(string name)
        {
            throw new NotImplementedException();
        }
        public IEnumElementInfo? FindEnumElement(string name)
        {
            throw new NotImplementedException();
        }
        public IEnumElementInfo? FindEnumElement(int value)
        {
            throw new NotImplementedException();
        }
        public HashSet<ITypeInfo> m_bases = new();
        public List<IConstructorInfo> m_constructors = new();
        public Dictionary<string, IPropertyInfo> m_properties = new();
        public Dictionary<string, IMethodInfo> m_methods = new();
        public List<string> m_propertyOrder = new();
        public List<string> m_methodOrder = new();
    }

}

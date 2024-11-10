namespace OctbitEngine.Runtime
{
    /// <summary>
    /// タグ情報
    /// </summary>
    public interface ITagInfo
    {
        IReadOnlyDictionary<string, string> Tags { get; }
    }

    /// <summary>
    /// Enum要素情報
    /// </summary>
    public interface IEnumElementInfo : ITagInfo
    {
        string Name { get; }
        int Index { get; }
        int Value { get; }
    }

    /// <summary>
    /// 引数情報
    /// </summary>
    public interface IArgumentInfo : ITagInfo
    {
        ITypeInfo TypeInfo { get; }
        string Name { get; }
    }

    /// <summary>
    /// コンストラクタ情報
    /// </summary>
    public interface IConstructorInfo : ITagInfo
    {
        IReadOnlyList<IArgumentInfo> Arguments { get; }
        object? Invoke(params object?[] args);
    }

    /// <summary>
    /// プロパティ情報
    /// </summary>
    public interface IPropertyInfo : ITagInfo
    {
        ITypeInfo TypeInfo { get; }
        string Name { get; }
        bool CanRead { get; }
        bool CanWrite { get; }
        bool CanReadWrite
            => CanRead && CanWrite;

        object? GetValue(object obj);
        bool SetValue(object obj, object? value);
    }

    /// <summary>
    /// メソッド情報
    /// </summary>
    public interface IMethodInfo : ITagInfo
    {
        string Name { get; }
        ITypeInfo ReturnType { get; }
        IReadOnlyList<IArgumentInfo> Arguments { get; }
        object? Invoke(params object?[] args);
    }

    /// <summary>
    /// 型情報
    /// </summary>
    public interface ITypeInfo : ITagInfo
    {
        string Name { get; }
        string ShortName { get; }
        ISet<ITypeInfo> Bases { get; }
        IReadOnlyList<IConstructorInfo> Constructors { get; }
        IReadOnlyDictionary<string,IPropertyInfo> Properties { get; }
        IReadOnlyDictionary<string, IMethodInfo> Methods { get; }

        IReadOnlyList<string> PropertyOrder { get; }
        IReadOnlyList<string> MethodOrder { get; }

        bool IsEnum { get; }



        bool IsBaseClassOf(ITypeInfo type);
        bool IsSuperClassOf(ITypeInfo type);

        IConstructorInfo? FindConstructor(params ITypeInfo[] args);
        IPropertyInfo? FindProperty(string name);
        IMethodInfo? FindMethod(string name);
        IEnumElementInfo? FindEnumElement(string name);
        IEnumElementInfo? FindEnumElement(int value);
    }


}

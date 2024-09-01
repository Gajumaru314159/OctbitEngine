using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace OctbitEngine.Runtime
{

    public interface IObject : INotifyPropertyChanged
    {
        ITypeInfo TypeInfo { get; }
    }

    public interface IPrimitiveObject : IObject
    {

        object GetValue(object fallback);
        T GetValue<T>(T fallback) where T : notnull;

        bool SetValue(object value);
        bool SetValue<T>(T value) where T : notnull;
    }

    public interface IClassObject : IObject
    {
        IReadOnlyDictionary<string, object> Properties { get; }

        object GetValue(object fallback,[CallerMemberName] string? name = null);
        T GetValue<T>(T fallback,[CallerMemberName] string? name = null) where T : notnull;

        bool SetValue(object value, [CallerMemberName] string? name = null);
        bool SetValue<T>(T value, [CallerMemberName] string? name = null) where T : notnull;
    }

}

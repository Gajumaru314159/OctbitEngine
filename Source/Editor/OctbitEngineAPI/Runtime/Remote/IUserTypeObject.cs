using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace OctbitEngine.Runtime
{
    public interface IUserTypeObject : INotifyPropertyChanged
    {
        IReadOnlyDictionary<string, object> Properties { get; }

        object? GetValue([CallerMemberName] string? name = null);
        T GetValue<T>([CallerMemberName] string? name = null) where T : notnull;

        bool SetValue(object? value, [CallerMemberName] string? name = null);
        bool SetValue<T>(T value, [CallerMemberName] string? name = null) where T : notnull;

        void Serialize(BinaryWriter writer);
        void Deserialize(BinaryReader reader);
        void Serialize(BinaryWriter writer,string propertyName);
        void Deserialize(BinaryReader reader, string propertyName);
    }

}

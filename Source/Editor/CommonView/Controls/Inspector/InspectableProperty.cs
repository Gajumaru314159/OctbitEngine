namespace CommonView.Controls
{
    public interface IInspectableProperty
    {
        object Owner { get; }
        string Name { get; }
        Type Type { get; }
        object? Value { get; set; }
        bool CanWrite { get; }
        bool CanRead { get; }
        IReadOnlyDictionary<string,string> Tags { get; }
    }

}

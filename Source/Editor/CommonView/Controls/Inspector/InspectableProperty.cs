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

    public class InspectableProperty : IInspectableProperty
    {
        public object Owner => throw new NotImplementedException();

        public string Name =>string.Empty;

        public Type Type => throw new NotImplementedException();

        public object? Value { get => throw new NotImplementedException(); set => throw new NotImplementedException(); }

        public bool CanWrite => throw new NotImplementedException();

        public bool CanRead => throw new NotImplementedException();

        public IReadOnlyDictionary<string, string> Tags => throw new NotImplementedException();
    }

}

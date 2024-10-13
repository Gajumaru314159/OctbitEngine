namespace CommonView.Controls
{
    public abstract class InspectableProperty : Inspectable
    {
        protected InspectableProperty()
        {

        }

        public abstract string DisplayName { get; }
        public abstract string Name { get; }
        public abstract Type Type { get; }
        public abstract object? Value { get; set; }
        public abstract bool CanRead{ get; }
        public abstract bool CanWrite { get; }
        public abstract IReadOnlyDictionary<string, string> Tags { get; }
    }

}

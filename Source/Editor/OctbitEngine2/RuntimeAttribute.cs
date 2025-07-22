namespace OctbitEngine2
{
    internal class RuntimeAttribute : Attribute
    {
        public string Name { get; }
        public int Hash { get; }
        public RuntimeAttribute(string name) {
            Name = name;
            Hash = name.GetHashCode();
        }
    }
}

namespace OctbitEngine.Asset
{

    [AttributeUsage(AttributeTargets.All, AllowMultiple = true)]
    public class AssetEditorAttribute : System.Attribute
    {
        public Type Type { get; }

        public AssetEditorAttribute(Type type)
        {
            Type = type;
        }
    }
}

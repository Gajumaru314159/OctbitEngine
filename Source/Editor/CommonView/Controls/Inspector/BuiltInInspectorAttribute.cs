using Livet;

namespace CommonView.Controls
{
    [AttributeUsage(AttributeTargets.Class|AttributeTargets.Struct|AttributeTargets.Enum, AllowMultiple = true)]
    public sealed class BuiltInInspectorAttribute : Attribute
    {
        public Type[] View { get; }
        public BuiltInInspectorAttribute(params Type[] view)
        {
            View = view;
        }

    }
}

namespace Common.Attribute
{

    [AttributeUsage(AttributeTargets.All, AllowMultiple = true)]
    public class TagAttribute : System.Attribute
    {
        public string Key { get; }
        public string Value { get; }

        public TagAttribute(string key, string value)
        {
            Key = key;
            Value = value;
        }
    }

    [AttributeUsage(AttributeTargets.All)]
    public class DisplayNameAttribute : TagAttribute
    {
        public string DisplayName => Value;

        public DisplayNameAttribute(string displayName)
            : base("DisplayName", displayName)
        {
        }
    }

    [AttributeUsage(AttributeTargets.All)]
    public class TooltipAttribute : TagAttribute
    {
        public string Tooltip => Value;

        public TooltipAttribute(string tooltip)
            : base("Tooltip", tooltip)
        {
        }
    }

    [AttributeUsage(AttributeTargets.Property)]
    public class OrderAttribute : System.Attribute
    {
        public int Order { get; }
        public OrderAttribute(int order)
        {
            Order = order;
        }
    }

    [AttributeUsage(AttributeTargets.Property)]
    public class SpaceAttribute : System.Attribute
    {
        public int Space { get; }
        public SpaceAttribute(int space)
        {
            Space = space;
        }
    }

    [AttributeUsage(AttributeTargets.Property)]
    public class RangeAttribute : System.Attribute
    {
        public double Min { get; }
        public double Max { get; }

        public RangeAttribute(double min, double max)
        {
            Min = min;
            Max = max;
        }
    }

    [AttributeUsage(AttributeTargets.Property)]
    public class MultilineAttribute : System.Attribute
    {
        public int Multiline{ get; }

        public MultilineAttribute(int multilne)
        {
            Multiline = multilne;
        }
    }
}

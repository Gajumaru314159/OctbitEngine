using System.Windows;
using System.Windows.Controls;

namespace CommonView.Controls
{
    [BuiltInInspector(typeof(bool))]
    public class BoolInspectorControl : Control
    {
        static BoolInspectorControl()
        {
            DefaultStyleKeyProperty.OverrideMetadata<BoolInspectorControl>();
        }
    }

    [BuiltInInspector(typeof(int), typeof(uint), typeof(short), typeof(ushort), typeof(long), typeof(ulong))]
    public class IntegerInspectorControl : Control
    {
        static IntegerInspectorControl()
        {
            DefaultStyleKeyProperty.OverrideMetadata<IntegerInspectorControl>();
        }
    }

    [BuiltInInspector(typeof(float), typeof(double))]
    public class DecimalInspectorControl : Control
    {
        static DecimalInspectorControl()
        {
            DefaultStyleKeyProperty.OverrideMetadata<DecimalInspectorControl>();
        }
    }

    [BuiltInInspector(typeof(string))]
    public class StringInspectorControl : Control
    {
        static StringInspectorControl()
        {
            DefaultStyleKeyProperty.OverrideMetadata<StringInspectorControl>();
        }
    }
}

using System.Windows;
using System.Windows.Controls;

namespace CommonView.Controls
{
    public class InspectorItemDataSelector : DataTemplateSelector
    {
        static InspectorItemDataSelector()
        {
            
        }

        public override DataTemplate? SelectTemplate(object item, DependencyObject container)
        {
            if (container is not FrameworkElement f) return null;
            if (item is InspectableProperty p)
            {
                return f.FindResource("InspectablePropertyTemplate") as DataTemplate;
            }
            if (item is InspectableMethod m)
            {
                return f.FindResource("InspectableMethodTemplate") as DataTemplate;
            }
            if (item is InspectableObject o)
            {
                return f.FindResource("InspectableObjectTemplate") as DataTemplate;
            }
            // TODO List
            // TODO Dictionary
            return null;
        }

    }


    public class InspectorControlDataSelector : DataTemplateSelector
    {
        static InspectorControlDataSelector()
        {

        }

        public override DataTemplate? SelectTemplate(object item, DependencyObject container)
        {
            if (container is not FrameworkElement f) return null;
            if (item is not InspectableProperty p) return null;

            if (p.Type == typeof(bool))
            {
                return f.FindResource("BoolInspectorControl") as DataTemplate;
            }
            if (p.Type == typeof(int))
            {
                return f.FindResource("IntInspectorControl") as DataTemplate;
            }
            if (p.Type == typeof(float))
            {
                return f.FindResource("FloatInspectorControl") as DataTemplate;
            }
            if (p.Type == typeof(string))
            {
                return f.FindResource("StringInspectorControl") as DataTemplate;
            }
            if (p.Type == typeof(Common.Math.Vector3))
            {
                return f.FindResource("Vector3InspectorControl") as DataTemplate;
            }
            if (p.Type.IsEnum)
            {
                return f.FindResource("EnumInspectorControl") as DataTemplate;
            }
            return null;
        }

    }
}

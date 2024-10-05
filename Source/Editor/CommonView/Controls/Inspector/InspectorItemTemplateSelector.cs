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
            if (item is not FrameworkElement f) return null;
            if (item is IInspectableProperty p)
            {
                return f.FindResource("PropertyTemplate") as DataTemplate;
            }
            if (item is IInspectableObject o)
            {
                return f.FindResource("ObjectTemplate") as DataTemplate;
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
            if (item is not FrameworkElement f) return null;
            if (item is not IInspectableProperty p) return null;

            if (p.Type == typeof(bool))
            {
                return f.FindResource("InspectorBoolTemplate") as DataTemplate;
            }
            if (p.Type == typeof(int))
            {
                return f.FindResource("InspectorBoolTemplate") as DataTemplate;
            }
            return null;
        }

    }
}

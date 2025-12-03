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
            if (item is Editor p)
            {
                return f.FindResource("InspectablePropertyTemplate") as DataTemplate;
            }
            if (item is MethodEditor m)
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

}

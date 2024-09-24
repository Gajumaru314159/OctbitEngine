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
            if (_dataTemplates.TryGetValue(item.GetType(), out var dataTemplate))
            {
                return dataTemplate;
            }
            return null;
        }

        private static Dictionary<Type, DataTemplate> _dataTemplates = new Dictionary<Type, DataTemplate>();
    }
}

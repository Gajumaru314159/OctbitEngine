using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;

namespace CommonView.Controls
{
    [ContentProperty(nameof(Templates))]
    public class GenericDataTemplateSelector : DataTemplateSelector
    {
        public Collection<DataTemplate> Templates { get; }
            = new Collection<DataTemplate>();

        public override DataTemplate? SelectTemplate(object item, DependencyObject container)
        {
            if (item == null) return null;

            var itemType = item.GetType();
            foreach (var template in Templates)
            {
                if (template.DataType is Type dataType && dataType.IsAssignableFrom(itemType))
                {
                    return template;
                }
            }
            return base.SelectTemplate(item, container);
        }
    }
}

using CommonView.Controls;
using System.Globalization;
using System.Windows;
using System.Windows.Data;
using System.Windows.Markup;

namespace CommonView
{
    public class AddConverter : MarkupExtension, IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if(value is not double doubleValue) return DependencyProperty.UnsetValue;
            if(!double.TryParse(parameter.ToString(),out double add)) return DependencyProperty.UnsetValue;
            return doubleValue + add;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return DependencyProperty.UnsetValue;
        }
        public override object ProvideValue(IServiceProvider serviceProvider)
        {
            return this;
        }
    }
}

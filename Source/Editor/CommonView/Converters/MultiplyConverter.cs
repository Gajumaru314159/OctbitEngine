using System.Globalization;
using System.Windows;
using System.Windows.Data;

namespace CommonView
{
    public class MultiplyConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if(value is not double doubleValue) return DependencyProperty.UnsetValue;
            if(parameter is not double multiplyValue) return DependencyProperty.UnsetValue;
            return doubleValue * multiplyValue;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return DependencyProperty.UnsetValue;
        }
    }
}

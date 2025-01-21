using System.Globalization;
using System.Windows;
using System.Windows.Data;
using System.Windows.Markup;

namespace CommonView
{
    /// <summary>
    /// インスペクタをネストしたときに表示する左端のライン調整用のコンバータ
    /// </summary>
    public class InspectorBorderConverter : MarkupExtension, IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is not double doubleValue) return new Thickness(0, 0, 0, 0);
            return new Thickness(doubleValue-7, 0, 0, 0);
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

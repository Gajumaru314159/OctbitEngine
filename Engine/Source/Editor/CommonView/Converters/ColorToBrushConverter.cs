using Common.Graphics;
using System.Globalization;
using System.Windows;
using System.Windows.Data;
using System.Windows.Markup;
using System.Windows.Media;

namespace CommonView
{
    public class ColorToBrushConverter : MarkupExtension, IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if(value is not Common.Graphics.Color color) return DependencyProperty.UnsetValue;
            float alpha = color.A;
            if(parameter is string mode && mode == "Opaque") alpha = 1.0f;
            return new SolidColorBrush(System.Windows.Media.Color.FromScRgb(alpha, color.R,color.G,color.B));
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

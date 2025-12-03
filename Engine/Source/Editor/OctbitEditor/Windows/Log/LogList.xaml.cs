using System.Globalization;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Markup;
using System.Windows.Media;
using Common.Log;

namespace OctbitEditor
{

    public class LogLevelToBrushConverter : IValueConverter
    {
        public Brush? ErrorBrush { get; set; }
        public Brush? WarnBrush { get; set; }
        public Brush? InfoBrush { get; set; }
        public Brush? TraceBrush { get; set; }

        public object? Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if (value is not LogLevel level) return DependencyProperty.UnsetValue;
            return value switch
            {
                LogLevel.Error => ErrorBrush??DependencyProperty.UnsetValue,
                LogLevel.Warn => WarnBrush??DependencyProperty.UnsetValue,
                LogLevel.Info => InfoBrush??DependencyProperty.UnsetValue,
                LogLevel.Trace => TraceBrush??DependencyProperty.UnsetValue,
                _ => DependencyProperty.UnsetValue
            };
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            return DependencyProperty.UnsetValue;
        }
    }

    /// <summary>
    /// LogList.xaml の相互作用ロジック
    /// </summary>
    public partial class LogList : UserControl
    {
        public LogList()
        {
            InitializeComponent();
        }
    }
}

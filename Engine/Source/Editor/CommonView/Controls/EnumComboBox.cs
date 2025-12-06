using System.Windows;
using System.Windows.Controls;

namespace CommonView.Controls
{
    public class EnumComboBox : ContentControl
    {
        public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
                nameof(Value),
                typeof(object),
                typeof(EnumComboBox),
                new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, new PropertyChangedCallback(OnValueChanged)));

        public object Value
        {
            get { return GetValue(ValueProperty); }
            set { SetValue(ValueProperty, value); }
        }

        private ComboBox m_combo;

        public class EnumElement{
        
        }

        public EnumComboBox()
        {
            m_combo = new ComboBox();
            //m_combo.DisplayMemberPath = "DisplayName";
            Content = m_combo;
        }

        private static void OnValueChanged(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            if (obj is not EnumComboBox ctrl) return;
            if (e.OldValue?.GetType() == e.NewValue?.GetType()) return;
            
            if(e.NewValue is null || e.NewValue.GetType().IsEnum==false)
            {
                ctrl.m_combo.ItemsSource = Array.Empty<object>();
                return;
            }

            ctrl.m_combo.ItemsSource = Enum.GetValues(e.NewValue.GetType());
            ctrl.m_combo.SelectedItem = e.NewValue;
        }

    }
}

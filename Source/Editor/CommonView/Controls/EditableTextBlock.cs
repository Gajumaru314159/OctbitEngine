using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows;

using TextBox = System.Windows.Controls.TextBox;
using MouseEventArgs = System.Windows.Input.MouseEventArgs;
using KeyEventArgs = System.Windows.Input.KeyEventArgs;
using Binding = System.Windows.Data.Binding;

namespace CommonView
{
    public class EditableTextBlock : ContentControl
    {
        private readonly Grid grid = new();
        private readonly TextBox textBox = new();
        private readonly TextBlock textBlock = new();
        private string previewText = string.Empty;
        private bool isEditMode = false;

        public string Text
        {
            get { return (string)GetValue(TextProperty); }
            set { SetValue(TextProperty, value); }
        }

        public static readonly DependencyProperty TextProperty = DependencyProperty.Register(
                nameof(Text),
                typeof(string),
                typeof(EditableTextBlock),
                new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));


        public EditableTextBlock()
        {
            InitializeControl();

            MouseDoubleClick += EditableTextBlock_MouseDoubleClick;
            textBox.KeyDown += TextBox_KeyDown;
            textBox.LostFocus += TextBox_LostFocus;
        }


        private void InitializeControl()
        {
            textBox.Padding = new Thickness(1.0);
            textBox.Visibility = Visibility.Hidden;
            textBox.VerticalAlignment = VerticalAlignment.Center;
            textBox.VerticalContentAlignment = VerticalAlignment.Center;

            textBlock.Visibility = Visibility.Visible;
            textBlock.VerticalAlignment = VerticalAlignment.Center;

            grid.Children.Add(textBlock);
            grid.Children.Add(textBox);
            Content = grid;
            Focusable = false;
            grid.Focusable = false;
            textBlock.Focusable = false;
            MinWidth = 10;

            Binding binding = new(nameof(Text))
            {
                Source = this,
                UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged,
            };
            textBox.SetBinding(TextBox.TextProperty, binding);
            textBlock.SetBinding(TextBlock.TextProperty, binding);
        }

        private void EditableTextBlock_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (!isEditMode)
            {
                previewText = textBlock.Text;
                isEditMode = true;
                OnIsEditModeChanged(isEditMode);
                textBox.Focus();
                textBox.SelectAll();
                e.Handled = true;
            }
        }

        private void TextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                isEditMode = false;
                OnIsEditModeChanged(isEditMode);
                e.Handled = true;
            }
            else if (e.Key == Key.Escape)
            {
                textBox.Text = previewText;
                isEditMode = false;
                OnIsEditModeChanged(isEditMode);
                e.Handled = true;
            }
        }

        private void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            isEditMode = false;
            OnIsEditModeChanged(isEditMode);
            e.Handled = true;
        }

        private void OnIsEditModeChanged(bool value)
        {
            textBlock.Visibility = value ? Visibility.Hidden : Visibility.Visible;
            textBox.Visibility = value ? Visibility.Visible : Visibility.Hidden;
        }
    }
}

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
        private readonly Grid m_grid = new();
        private readonly TextBox m_textBox = new();
        private readonly TextBlock m_textBlock = new();
        private string m_previousText = string.Empty;
        private bool m_isEditMode = false;


        public static readonly DependencyProperty TextProperty = DependencyProperty.Register(
                nameof(Text),
                typeof(string),
                typeof(EditableTextBlock),
                new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));


        public static readonly DependencyProperty TextWrappingProperty = DependencyProperty.Register(
                nameof(TextWrapping),
                typeof(TextWrapping),
                typeof(EditableTextBlock),
                new FrameworkPropertyMetadata(TextWrapping.NoWrap, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, (obj, e) =>
                {
                    if(obj is EditableTextBlock et)
                    {
                        et.m_textBlock.TextWrapping = (TextWrapping)e.NewValue;
                        et.m_textBox.TextWrapping = (TextWrapping)e.NewValue;
                    }
                }));

        public string Text
        {
            get { return (string)GetValue(TextProperty); }
            set { SetValue(TextProperty, value); }
        }

        public TextWrapping TextWrapping
        {
            get { return (TextWrapping)GetValue(TextWrappingProperty); }
            set { SetValue(TextWrappingProperty, value); }
        }

        public EditableTextBlock()
        {
            InitializeControl();

            MouseDoubleClick += EditableTextBlock_MouseDoubleClick;
            m_textBox.PreviewKeyDown += TextBox_KeyDown;
            m_textBox.LostFocus += TextBox_LostFocus;   
            m_textBox.LostKeyboardFocus += TextBox_LostFocus;
        }


        private void InitializeControl()
        {
            m_textBox.Padding = new Thickness(0);
            m_textBox.Visibility = Visibility.Hidden;
            m_textBox.VerticalAlignment = VerticalAlignment.Center;
            m_textBox.VerticalContentAlignment = VerticalAlignment.Center;

            m_textBox.SetBinding(TextBox.TextWrappingProperty, nameof(TextWrapping));

            m_textBlock.Padding = new Thickness(3,0,0,0);
            m_textBlock.Visibility = Visibility.Visible;
            m_textBlock.VerticalAlignment = VerticalAlignment.Center;
            m_textBlock.TextTrimming = TextTrimming.CharacterEllipsis;

            m_grid.Children.Add(m_textBlock);
            m_grid.Children.Add(m_textBox);
            Content = m_grid;
            Focusable = false;
            m_grid.Focusable = false;
            m_textBlock.Focusable = false;
            MinWidth = 10;

            {
                Binding binding = new(nameof(Text))
                {
                    Source = this,
                    UpdateSourceTrigger = UpdateSourceTrigger.Explicit,
                };
                m_textBox.SetBinding(TextBox.TextProperty, binding);
                m_textBlock.SetBinding(TextBlock.TextProperty, binding);
            }
        }

        private void UpdateText()
        {
            var binding = BindingOperations.GetBindingExpression(m_textBox, TextBox.TextProperty);
            binding?.UpdateSource();
            m_textBox.TextWrapping = TextWrapping;
            m_textBlock.TextWrapping = TextWrapping;
        }

        private void EditableTextBlock_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (m_isEditMode) return;

            m_previousText = m_textBlock.Text;
            m_isEditMode = true;
            OnIsEditModeChanged();
            m_textBox.Focus();
            m_textBox.SelectAll();
            e.Handled = true;
        }

        private void TextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                m_isEditMode = false;
                UpdateText();
                OnIsEditModeChanged();
                e.Handled = true;
            }
            else if (e.Key == Key.Escape || e.Key == Key.Tab)
            {
                m_textBox.Text = m_previousText;
                m_isEditMode = false;
                OnIsEditModeChanged();
                e.Handled = true;
            }
        }

        private void TextBox_LostFocus(object sender, RoutedEventArgs e)
        {
            m_isEditMode = false;
            OnIsEditModeChanged();
            e.Handled = true;
        }

        private void OnIsEditModeChanged()
        {
            m_textBlock.Visibility = m_isEditMode ? Visibility.Hidden : Visibility.Visible;
            m_textBox.Visibility = m_isEditMode ? Visibility.Visible : Visibility.Hidden;
        }
    }
}

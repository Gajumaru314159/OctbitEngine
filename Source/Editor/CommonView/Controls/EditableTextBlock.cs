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


        public static readonly DependencyProperty TextProperty = DependencyProperty.Register(
                nameof(Text),
                typeof(string),
                typeof(EditableTextBlock),
                new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));

        public static readonly DependencyProperty IsEdittingProperty = DependencyProperty.Register(
                nameof(IsEditting),
                typeof(bool),
                typeof(EditableTextBlock),
                new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, new PropertyChangedCallback(OnIsEdittingChanged)));

        public static readonly DependencyProperty IsEditableProperty = DependencyProperty.Register(
                nameof(IsEditable),
                typeof(bool),
                typeof(EditableTextBlock),
                new PropertyMetadata(true));

        public string Text
        {
            get { return (string)GetValue(TextProperty); }
            set { SetValue(TextProperty, value); }
        }
        public bool IsEditting
        {
            get { return (bool)GetValue(IsEdittingProperty); }
            set { SetValue(IsEdittingProperty, value); }
        }
        public bool IsEditable
        {
            get { return (bool)GetValue(IsEditableProperty); }
            set { SetValue(IsEditableProperty, value); }
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public EditableTextBlock()
        {
            InitializeControl();

            MouseDoubleClick += OnDoubleClicked;
            m_textBox.PreviewKeyDown += OnKeyDown;
            m_textBox.LostFocus += OnLostFocus;   
            m_textBox.LostKeyboardFocus += OnLostFocus;
        }

        /// <summary>
        /// コントロールを初期化する
        /// </summary>
        private void InitializeControl()
        {
            m_textBox.Padding = new Thickness(0);
            m_textBox.Visibility = Visibility.Hidden;
            m_textBox.VerticalAlignment = VerticalAlignment.Center;
            m_textBox.VerticalContentAlignment = VerticalAlignment.Center;

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


        private void OnDoubleClicked(object sender, MouseEventArgs e)
        {
            if (IsEditable == false) return;
            IsEditting = true;
            e.Handled = true;
        }

        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            // 確定
            if (e.Key == Key.Enter)
            {
                CommitEdit();
                e.Handled = true;
            }
            
            // キャンセルまたはフォーカス切り替え
            if (e.Key == Key.Escape || e.Key == Key.Tab)
            {
                CancelEdit();
                e.Handled = true;
            }

            // F2キーで編集開始
            if (e.Key == Key.F2)
            {
                if (IsEditable)
                {
                    IsEditting = true;
                    e.Handled = true;
                }
            }
        }

        private void OnLostFocus(object sender, RoutedEventArgs e)
        {
            CancelEdit();
            e.Handled = true;
        }

        private static void OnIsEdittingChanged(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            if (obj is not EditableTextBlock etb) return;
            if (e.NewValue is not bool isEditting) return;

            if (isEditting)
            {
                etb.BeginEdit();
            }
            else
            {
                etb.CancelEdit();
            }
        }

        private void BeginEdit()
        {
            if (IsEditable==false) return;
            m_previousText = m_textBlock.Text;
            m_textBox.Text = Text;
            UpdateControlVisibility();
            m_textBox.Focus();
            m_textBox.SelectAll();
        }

        private void CommitEdit()
        {
            if (IsEditable)
            {
                var binding = BindingOperations.GetBindingExpression(m_textBox, TextBox.TextProperty);
                binding?.UpdateSource();
            }
            UpdateControlVisibility();
            IsEditting = false;
        }

        private void CancelEdit()
        {
            m_textBox.Text = m_previousText;
            UpdateControlVisibility();
            IsEditting = false;
        }

        private void UpdateControlVisibility()
        {
            m_textBlock.Visibility = IsEditting ? Visibility.Hidden : Visibility.Visible;
            m_textBox.Visibility = IsEditting ? Visibility.Visible : Visibility.Hidden;
        }

    }
}

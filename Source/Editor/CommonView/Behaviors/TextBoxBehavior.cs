using Microsoft.Xaml.Behaviors;
using System.Windows.Data;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Input;

namespace CommonView.Behaviors
{
    public class EnterKeyBehavior : Behavior<TextBox>
    {
        private static readonly DependencyProperty s_changeFocusWhenEnterKeyPressedProperty
            = DependencyProperty.Register(nameof(ChangeFocusWhenEnterKeyPressed), typeof(bool), typeof(EnterKeyBehavior), new PropertyMetadata(true));

        private IInputElement? _previousFocusedElement;
        public bool ChangeFocusWhenEnterKeyPressed
        {
            get => (bool)GetValue(s_changeFocusWhenEnterKeyPressedProperty);
            set => SetValue(s_changeFocusWhenEnterKeyPressedProperty, (bool)value);
        }
        protected override void OnAttached()
        {
            base.OnAttached();
            AssociatedObject.PreviewKeyDown += OnPreViewKeyDown;
            AssociatedObject.GotKeyboardFocus +=OnGotKeyboardFocus;
        }

        protected override void OnDetaching()
        {
            base.OnDetaching();
            AssociatedObject.PreviewKeyDown -= OnPreViewKeyDown;
            AssociatedObject.GotKeyboardFocus -=OnGotKeyboardFocus;
        }

        private void OnPreViewKeyDown(object? sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                var binding = BindingOperations.GetBindingExpression(AssociatedObject, TextBox.TextProperty);
                binding?.UpdateSource();

                if (ChangeFocusWhenEnterKeyPressed)
                {
                    // var direction = (Keyboard.Modifiers == ModifierKeys.Shift) ? FocusNavigationDirection.Previous : FocusNavigationDirection.Next;
                    // AssociatedObject.MoveFocus(new TraversalRequest(direction));
                    Keyboard.ClearFocus(); 
                    
                    if (_previousFocusedElement != null)
                    {
                        (_previousFocusedElement as UIElement)?.Focus();
                    }
                }
            }
        }
        private void OnGotKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e)
        {
            _previousFocusedElement = e.OldFocus;
        }

    }

    public class SelectAllOnFocusBehavior : Behavior<TextBox>
    {
        protected override void OnAttached()
        {
            base.OnAttached();
            AssociatedObject.GotKeyboardFocus += OnGotKeyboardFocus;
            AssociatedObject.GotMouseCapture += OnGotMouseCapture;
        }

        protected override void OnDetaching()
        {
            base.OnDetaching();
            AssociatedObject.GotKeyboardFocus -= OnGotKeyboardFocus;
            AssociatedObject.GotMouseCapture -= OnGotMouseCapture;
        }

        private void OnGotKeyboardFocus(object sender, RoutedEventArgs e)
        {
            AssociatedObject.SelectAll();
        }

        private void OnGotMouseCapture(object sender, RoutedEventArgs e)
        {
            if (!AssociatedObject.IsKeyboardFocusWithin)
            {
                AssociatedObject.SelectAll();
            }
        }
    }
}

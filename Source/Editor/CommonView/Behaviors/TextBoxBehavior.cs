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

        public bool ChangeFocusWhenEnterKeyPressed
        {
            get => (bool)GetValue(s_changeFocusWhenEnterKeyPressedProperty);
            set => SetValue(s_changeFocusWhenEnterKeyPressedProperty, (bool)value);
        }
        protected override void OnAttached()
        {
            base.OnAttached();
            AssociatedObject.PreviewKeyDown += OnPreViewKeyDown;
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
                }
            }
        }
        
    }
}

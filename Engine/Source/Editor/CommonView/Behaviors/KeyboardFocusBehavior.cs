using Microsoft.Xaml.Behaviors;
using System.Windows;
using System.Windows.Input;

namespace CommonView.Behaviors
{
    public class KeyboardFocusBehavior : Behavior<UIElement>
    {
        public static readonly DependencyProperty TargetProperty =
            DependencyProperty.Register(nameof(Target), typeof(UIElement), typeof(KeyboardFocusBehavior));

        public UIElement Target
        {
            get { return (UIElement)GetValue(TargetProperty); }
            set { SetValue(TargetProperty, value); }
        }

        public static readonly DependencyProperty InputGestureProperty =
            DependencyProperty.Register(nameof(InputGesture), typeof(KeyGesture), typeof(KeyboardFocusBehavior));

        public KeyGesture InputGesture
        {
            get { return (KeyGesture)GetValue(InputGestureProperty); }
            set { SetValue(InputGestureProperty, value); }
        }

        protected override void OnAttached()
        {
            base.OnAttached();
            AssociatedObject.KeyDown += OnKeyDown;
        }

        protected override void OnDetaching()
        {
            base.OnDetaching();
            AssociatedObject.KeyDown -= OnKeyDown;
        }

        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            if (InputGesture != null && InputGesture.Matches(null, e) && Target != null)
            {
                Keyboard.Focus(Target);
                e.Handled = true;
            }
        }
    }
}

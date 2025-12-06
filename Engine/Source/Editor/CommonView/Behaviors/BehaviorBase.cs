using Microsoft.Xaml.Behaviors;
using System.Windows;

namespace CommonView
{
    public abstract class BehaviorBase<T1, T2> : Behavior<T2> where T2 : DependencyObject
    {
        public static DependencyProperty TargetProperty = DependencyProperty.Register(
                       nameof(Target),
                       typeof(T1),
                       typeof(BehaviorBase<T1, T2>),
                       new FrameworkPropertyMetadata(default(T1), FrameworkPropertyMetadataOptions.BindsTwoWayByDefault, null));

        public T1 Target
        {
            get => (T1)GetValue(TargetProperty);
            set => SetValue(TargetProperty, value);
        }

        protected override void OnAttached()
        {
            base.OnAttached();
            OnAttachedAction();
        }

        protected override void OnDetaching()
        {
            OnDetachingAction();
            base.OnDetaching();
        }

        protected abstract void OnAttachedAction();
        protected abstract void OnDetachingAction();
    }

}

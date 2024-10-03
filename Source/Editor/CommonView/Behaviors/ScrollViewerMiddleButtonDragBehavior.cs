using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace CommonView.Behaviors
{
    public class ScrollViewerMiddleButtonDragBehavior : Microsoft.Xaml.Behaviors.Behavior<ScrollViewer>
    {
        protected override void OnAttached()
        {
            base.OnAttached();

            AssociatedObject.PreviewMouseDown += AssociatedObjectOnPreviewMouseDown;
            AssociatedObject.PreviewMouseUp += AssociatedObjectOnPreviewMouseUp;
            AssociatedObject.PreviewMouseMove += AssociatedObjectOnPreviewMouseMove;
        }

        protected override void OnDetaching()
        {
            base.OnDetaching();

            AssociatedObject.PreviewMouseDown -= AssociatedObjectOnPreviewMouseDown;
            AssociatedObject.PreviewMouseUp -= AssociatedObjectOnPreviewMouseUp;
            AssociatedObject.PreviewMouseMove -= AssociatedObjectOnPreviewMouseMove;
        }

        private void AssociatedObjectOnPreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (IsEnabled == false)
                return;

            if (e.OriginalSource is DependencyObject control)
                if (control != AssociatedObject)
                    if (control.GetParentFast<ScrollViewer>() != AssociatedObject)
                        return;

            if (m_isInDragging)
            {
                CancelDrag();
            }
            else if (e.ChangedButton == MouseButton.Middle && e.ButtonState == MouseButtonState.Pressed)
            {
                if (m_isInDragging)
                    return;

                m_isInDragging = true;

                m_previousPos = e.GetPosition(ParentControl);

                // TODO カーソル対応

                AssociatedObject.CaptureMouse();

                e.Handled = true;
            }
        }

        private void AssociatedObjectOnPreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            if (IsEnabled == false)
                return;

            if (e.ChangedButton == MouseButton.Middle &&
                e.ButtonState == MouseButtonState.Released)
            {
                CancelDrag();
                e.Handled = true;
            }
        }

        private void AssociatedObjectOnPreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (IsEnabled == false)
                return;

            if (m_isInDragging == false)
                return;

            var currentPos = e.GetPosition(ParentControl);
            var diff = currentPos - m_previousPos;

            AssociatedObject.ScrollToVerticalOffset(AssociatedObject.VerticalOffset - diff.Y);
            AssociatedObject.ScrollToHorizontalOffset(AssociatedObject.HorizontalOffset - diff.X);

            m_previousPos = currentPos;

            e.Handled = true;
        }

        private void CancelDrag()
        {
            m_isInDragging = false;

            AssociatedObject.ReleaseMouseCapture();

        }

        private bool IsEnabled =>
            !(AssociatedObject.HorizontalScrollBarVisibility == ScrollBarVisibility.Disabled &&
              AssociatedObject.VerticalScrollBarVisibility == ScrollBarVisibility.Disabled);

        private IInputElement? ParentControl => AssociatedObject.Parent as IInputElement;

        private bool m_isInDragging;
        private Point m_previousPos;

    }
}
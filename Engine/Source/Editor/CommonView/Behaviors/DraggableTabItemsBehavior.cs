using Microsoft.Xaml.Behaviors;
using System.Collections;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace CommonView.Behaviors
{
    public class DraggableTabItemsBehavior : Behavior<TabControl>
    {
        private Point m_dragStartPoint;
        private TabItem? m_draggedTab;
        private bool m_isDragging;

        protected override void OnAttached()
        {
            base.OnAttached();
            AssociatedObject.AllowDrop = true;
            AssociatedObject.PreviewMouseLeftButtonDown += OnMouseLeftButtonDown;
            AssociatedObject.PreviewMouseMove += OnMouseMove;
            AssociatedObject.Drop += OnDrop;
            AssociatedObject.DragOver += OnDragOver;
        }

        protected override void OnDetaching()
        {
            base.OnDetaching();
            AssociatedObject.PreviewMouseLeftButtonDown -= OnMouseLeftButtonDown;
            AssociatedObject.PreviewMouseMove -= OnMouseMove;
            AssociatedObject.Drop -= OnDrop;
            AssociatedObject.DragOver -= OnDragOver;
        }

        private void OnMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            m_dragStartPoint = e.GetPosition(null);
            m_draggedTab = FindAncestor<TabItem>((DependencyObject)e.OriginalSource);
        }

        private void OnMouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton != MouseButtonState.Pressed || m_isDragging || m_draggedTab == null)
                return;

            var currentPos = e.GetPosition(null);
            if (Math.Abs(currentPos.X - m_dragStartPoint.X) > SystemParameters.MinimumHorizontalDragDistance ||
                Math.Abs(currentPos.Y - m_dragStartPoint.Y) > SystemParameters.MinimumVerticalDragDistance)
            {
                m_isDragging = true;
                var data = new DataObject(typeof(object), m_draggedTab.DataContext);
                DragDrop.DoDragDrop(m_draggedTab, data, DragDropEffects.Move);
                m_isDragging = false;
            }
        }

        private void OnDragOver(object sender, DragEventArgs e)
        {
            e.Effects = DragDropEffects.Move;
            e.Handled = true;
        }

        private void OnDrop(object sender, DragEventArgs e)
        {
            if (!(AssociatedObject.ItemsSource is IList itemsSource))
                return;

            var sourceData = e.Data.GetData(typeof(object));
            var targetTab = FindAncestor<TabItem>((DependencyObject)e.OriginalSource);
            if (sourceData == null || targetTab?.DataContext == null)
                return;

            var targetData = targetTab.DataContext;
            int oldIndex = itemsSource.IndexOf(sourceData);
            int newIndex = itemsSource.IndexOf(targetData);

            if (oldIndex < 0 || newIndex < 0 || oldIndex == newIndex)
                return;

            itemsSource.RemoveAt(oldIndex);
            itemsSource.Insert(newIndex, sourceData);
            AssociatedObject.SelectedItem = sourceData;

            m_draggedTab = null;
            e.Handled = true;
        }

        private static T? FindAncestor<T>(DependencyObject current) where T : DependencyObject
        {
            while (current != null)
            {
                if (current is T found)
                    return found;
                current = VisualTreeHelper.GetParent(current);
            }
            return null;
        }
    }
}

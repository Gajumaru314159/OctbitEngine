using System.Collections;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace CommonView
{
    public static class ViewHelper
    {
        public static ItemsControl? FindVisualParentItemsControl(DependencyObject? obj)
        {
            var self = obj;
            while (obj is not null)
            {
                obj = VisualTreeHelper.GetParent(obj);
                if (obj is ItemsControl parent) return parent;
            }
            return self as ItemsControl;
        }

        public static FrameworkElement FindTemplatedRoot(FrameworkElement element)
        {
            if (element.TemplatedParent is not FrameworkElement parent) return element;
            while (parent!.TemplatedParent is FrameworkElement pElement) parent = pElement;
            return parent;
        }
    }

    public class ItemMovement
    {
        public IList Source { get; }
        public IList Target { get; }
        public object MovedItem { get; }
        public int TargetIndex { get; }

        public ItemMovement(IList source, IList target, object movedItem, int targetIndex)
        {
            Source = source;
            Target = target;
            MovedItem = movedItem;
            TargetIndex = targetIndex;
        }

        public void MoveItem()
        {
            Source.Remove(MovedItem);
            Target.Insert(TargetIndex > Target.Count ? Target.Count : TargetIndex, MovedItem);
        }
    }


    public class DraggableItemsControlBehavior : BehaviorBase<Action<ItemMovement>, ItemsControl>
    {
        protected static DraggedObject? draggedObject;

        protected override void OnAttachedAction()
        {
            AssociatedObject.PreviewMouseLeftButtonDown += OnPreviewMouseLeftButtonDown;
            AssociatedObject.PreviewMouseLeftButtonUp += OnPreviewMouseLeftButtonUp;
            AssociatedObject.PreviewMouseMove += OnPreviewMouseMove;
            AssociatedObject.PreviewDragEnter += OnPreviewDragEnter;
            AssociatedObject.PreviewDragLeave += OnPreviewDragLeave;
            AssociatedObject.PreviewDrop += OnPreviewDrop;
        }

        protected override void OnDetachingAction()
        {
            AssociatedObject.PreviewMouseLeftButtonDown -= OnPreviewMouseLeftButtonDown;
            AssociatedObject.PreviewMouseLeftButtonUp -= OnPreviewMouseLeftButtonUp;
            AssociatedObject.PreviewMouseMove -= OnPreviewMouseMove;
            AssociatedObject.PreviewDragEnter -= OnPreviewDragEnter;
            AssociatedObject.PreviewDragLeave -= OnPreviewDragLeave;
            AssociatedObject.PreviewDrop -= OnPreviewDrop;
        }


        #region EventHandler
        private void OnPreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (sender is not FrameworkElement element || e.OriginalSource is not FrameworkElement source) return;
            var pos = GetPosition(e, element);
            var item = ViewHelper.FindTemplatedRoot(source);
            draggedObject = new DraggedObject(pos, item);
        }

        private void OnPreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            draggedObject = null;
        }

        private void OnPreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (sender is not FrameworkElement element || !(draggedObject?.IsDraggable(GetPosition(e, element)) ?? false)) return;
            DragDrop.DoDragDrop(element, draggedObject.DroppedItem, DragDropEffects.Move);
            draggedObject = null;
        }

        private void OnPreviewDragEnter(object sender, DragEventArgs e)
        {
            if (draggedObject is null) return;
            draggedObject.IsDroppable = true;
        }

        private void OnPreviewDragLeave(object sender, DragEventArgs e)
        {
            if (draggedObject is null) return;
            draggedObject.IsDroppable = false;
        }

        private void OnPreviewDrop(object sender, DragEventArgs e)
        {
            if (e.OriginalSource is not FrameworkElement element || draggedObject is null) return;

            var targetContainer = ViewHelper.FindTemplatedRoot(element);
            var sourceCollection = ViewHelper.FindVisualParentItemsControl(draggedObject.DroppedItem)?.ItemsSource as IList;
            var targetItemsControl = ViewHelper.FindVisualParentItemsControl(targetContainer);
            var targetCollection = targetItemsControl?.ItemsSource as IList;
            if (sourceCollection is null || targetCollection is null || targetItemsControl is null) return;
            if (!sourceCollection.GetType().IsGenericType || !targetCollection.GetType().IsGenericType) return;

            var sourceItemTypes = sourceCollection.GetType().GenericTypeArguments;
            var targetItemTypes = targetCollection.GetType().GenericTypeArguments;
            if (sourceItemTypes.Length != 1 || targetItemTypes.Length != 1) return;

            var droppedItem = draggedObject.DroppedItem.DataContext;
            if (droppedItem is null) return;

            if (sourceItemTypes[0] == targetItemTypes[0])
            {
                var index = targetItemsControl.ItemContainerGenerator.IndexFromContainer(targetContainer);
                index = index >= 0 ? index : targetCollection.Count;
                Target?.Invoke(new ItemMovement(sourceCollection, targetCollection, droppedItem, index));
                return;
            }

            var children = (targetContainer as ItemsControl)?.ItemsSource as IList;
            if (children is null) return;

            var childType = children.GetType();
            if (!childType.IsGenericType) return;

            var childTypeArgments = childType.GenericTypeArguments;
            if (childTypeArgments.Count() != 1 || childTypeArgments[0] != sourceItemTypes[0]) return;

            Target?.Invoke(new ItemMovement(sourceCollection, children, droppedItem, children.Count));
        }
        #endregion

        private static Point GetPosition(MouseEventArgs e, FrameworkElement element) => e.GetPosition(Window.GetWindow(element));


        protected class DraggedObject
        {
            public Point Start { get; }
            public FrameworkElement DroppedItem { get; }
            public bool IsDroppable { get; set; }

            private static readonly Vector minDragPoint = new Vector(SystemParameters.MinimumHorizontalDragDistance, SystemParameters.MinimumVerticalDragDistance);

            public DraggedObject(Point start, FrameworkElement droppedItem)
            {
                Start = start;
                DroppedItem = ViewHelper.FindTemplatedRoot(droppedItem);
            }

            public bool IsDraggable(Point current)
            {
                return (current - Start).Length >= minDragPoint.Length;
            }
        }
    }

}

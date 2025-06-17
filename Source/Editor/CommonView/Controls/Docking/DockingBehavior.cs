using Microsoft.Xaml.Behaviors;
using System.Collections;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace CommonView.Controls
{
    public class DockingBehavior : Behavior<DockingLayout>
    {
        private Point m_dragStartPoint;
        private TabItem? m_tab;
        private bool m_isDragging;

        class DockingDragObject
        {
            public object? Scope { get; init; }
            public IList? SourceList { get; init; }
            public object? Source { get; init; }
            public int Index => SourceList?.IndexOf(Source) ?? -1;
        }

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
            if (sender is not FrameworkElement f) return;
            m_dragStartPoint = e.GetPosition(null);
            m_tab = (e.OriginalSource as DependencyObject)?.GetParentFast<TabItem>();
        }

        private void OnMouseMove(object sender, MouseEventArgs e)
        {
            if (m_isDragging || m_tab == null)
                return;

            var currentPos = e.GetPosition(null);
            if (Math.Abs(currentPos.X - m_dragStartPoint.X) > SystemParameters.MinimumHorizontalDragDistance ||
                Math.Abs(currentPos.Y - m_dragStartPoint.Y) > SystemParameters.MinimumVerticalDragDistance)
            {
                if (e.LeftButton == MouseButtonState.Pressed)
                {
                    m_isDragging = true;

                    var dragObject = new DockingDragObject
                    {
                        Scope = null,
                        Source = m_tab.DataContext,
                        SourceList =  m_tab.GetParentFast<TabControl>()?.ItemsSource as IList
                    };

                    var data = new DataObject(dragObject.GetType(), dragObject);
                    DragDrop.DoDragDrop(m_tab, data, DragDropEffects.Move);

                    m_isDragging = false;
                }
            }
        }

        private void OnDragOver(object sender, DragEventArgs e)
        {
            e.Effects = e.Data.GetDataPresent(typeof(DockingDragObject)) ? DragDropEffects.Move : DragDropEffects.None;
            e.Handled = true;
        }

        private void OnDrop(object sender, DragEventArgs e)
        {
            var dragObject = e.Data.GetData(typeof(DockingDragObject)) as DockingDragObject;
            if (dragObject == null || dragObject.SourceList == null || dragObject.Source == null)
                return;
            // 異なるスコープであればキャンセル
            // if (dragObject.Scope != Scope) return;

            var dstTab = (e.OriginalSource as DependencyObject)?.GetParentFast<TabItem>();
            var dstCtrl = (e.OriginalSource as DependencyObject)?.GetParentFast<TabControl>();

            if (dstCtrl == null)
                return;

            var srcList = dragObject.SourceList;
            var dstList = dstCtrl.ItemsSource as IList;

            if (srcList == null || dstList == null)
                return;

            // ドロップ位置が[上下左右]の場合
            var host = dstCtrl?.Template.FindName("PART_SelectedContentHost", dstCtrl) as ContentPresenter;
            if(host != null)
            {
               var point = e.GetPosition(host);
                point.X/= Math.Max(host.ActualWidth,1);
                point.Y/= Math.Max(host.ActualHeight,1);

                bool a = point.Y < point.X;
                bool b = point.X < (1.0 - point.Y);
                bool isLeft = !a && b;
                bool isRight = a && !b;
                bool isTop = a && b;
                bool isBottom = !a && !b;

                double threshold = 0.2;
                bool isOutside = 0.5 < Math.Abs(point.X-0.5) || 0.5 < Math.Abs(point.Y-0.5);
                bool isCenter = Math.Abs(point.X-0.5) < threshold && Math.Abs(point.Y-0.5) < threshold;

                if (!isCenter && !isOutside)
                {
                    var split = (e.OriginalSource as DependencyObject)?.GetParentFast<SplitControl>();

                    DockingLayoutGroup? layout = split?.DataContext as DockingLayoutGroup;
                    DockingTabGroup? tabs = dstCtrl?.DataContext as DockingTabGroup;

                    if (layout != null && tabs != null)
                    {
                        int targetIndex = layout.Items.IndexOf(tabs);
                        if(targetIndex<0)return;

                        // 分割する場合は新しいTabGroupが必要
                        var newTabs = new DockingTabGroup();
                        newTabs.Add(dragObject.Source);

                        // ドロップ先のグループのOrientationを取得
                        var isHorizontal = layout.Orientation.Value == Orientation.Horizontal;
                        bool bNeedSplit = (isHorizontal && (isTop || isBottom)) || (!isHorizontal && (isLeft || isRight));
                        if (bNeedSplit)
                        {
                            var newLayout = new DockingLayoutGroup();
                            newLayout.Orientation.Value = isHorizontal ? Orientation.Vertical : Orientation.Horizontal;

                            var oldTabs = layout.Items[targetIndex];

                            if (isRight || isBottom)
                            {
                                newLayout.Add(newTabs);
                                newLayout.Add(oldTabs);
                            }
                            else
                            {
                                newLayout.Add(oldTabs);
                                newLayout.Add(newTabs);
                            }
                            
                            layout.Items.Insert(targetIndex, newLayout);
                            layout.Items.Remove(oldTabs);

                        } else
                        {
                            if (isRight || isBottom) targetIndex++;
                            layout.Items.Insert(targetIndex, newTabs);
                        }

                        dragObject.SourceList.Remove(dragObject.Source);
                    }

                    e.Handled = true;
                    return;
                }

            }


            // ドロップ位置が[タブ/中央]の場合
            int oldIndex = dragObject.Index;
            int newIndex = dstTab != null
                ? dstList.IndexOf(dstTab.DataContext)
                : dstList.Count;

            if (oldIndex < 0 || newIndex < 0)
                return;

            // 同一リストの場合は挿入操作でインデックスがずれるので補正
            // Removeを先にするとDockingTabGroupが削除されてしまうので先にInsertする
            if (srcList != dstList) { }
            else if (oldIndex < newIndex) newIndex = Math.Min(newIndex + 1, dstList.Count);
            else if (oldIndex > newIndex) oldIndex++;
            else if (oldIndex == newIndex) return;

            dstList.Insert(newIndex, dragObject.Source);
            srcList.RemoveAt(oldIndex);

            if(dstCtrl!=null) dstCtrl.SelectedItem = dragObject.Source;

            e.Handled = true;
        }

    }
}

using System.Collections;
using System.Collections.Specialized;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace CommonView.Controls
{
    public class SplitControl : Control
    {
        static SplitControl()
        {
            DefaultStyleKeyProperty.OverrideMetadata(
                typeof(SplitControl),
                new FrameworkPropertyMetadata(typeof(SplitControl)));
        }

        #region DependencyProperties

        public static readonly DependencyProperty ItemsSourceProperty =
            DependencyProperty.Register(
                nameof(ItemsSource),
                typeof(IEnumerable),
                typeof(SplitControl),
                new PropertyMetadata(null, OnItemsSourceChanged));

        public IEnumerable ItemsSource
        {
            get => (IEnumerable)GetValue(ItemsSourceProperty);
            set => SetValue(ItemsSourceProperty, value);
        }

        public static readonly DependencyProperty ContentTemplateSelectorProperty =
            DependencyProperty.Register(
                nameof(ContentTemplateSelector),
                typeof(DataTemplateSelector),
                typeof(SplitControl),
                new PropertyMetadata(null));

        public DataTemplateSelector ContentTemplateSelector
        {
            get => (DataTemplateSelector)GetValue(ContentTemplateSelectorProperty);
            set => SetValue(ContentTemplateSelectorProperty, value);
        }

        public static readonly DependencyProperty OrientationProperty =
            DependencyProperty.Register(
                nameof(Orientation),
                typeof(Orientation),
                typeof(SplitControl),
                new PropertyMetadata(Orientation.Horizontal, OnOrientationChanged));

        public Orientation Orientation
        {
            get => (Orientation)GetValue(OrientationProperty);
            set => SetValue(OrientationProperty, value);
        }

        public static readonly DependencyProperty GridWidthProperty =
            DependencyProperty.Register(
                nameof(GridWidth),
                typeof(double),
                typeof(SplitControl),
                new PropertyMetadata(5.0, OnGridWidthChanged));

        /// <summary>
        /// Splitter の太さ（ピクセル）
        /// </summary>
        public double GridWidth
        {
            get => (double)GetValue(GridWidthProperty);
            set => SetValue(GridWidthProperty, value);
        }

        #endregion

        private Grid? m_grid;

        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();
            m_grid = GetTemplateChild("PART_Grid") as Grid;
            RebuildGrid();
        }

        private static void OnItemsSourceChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            var ctrl = (SplitControl)d;
            ctrl.Unsubscribe((INotifyCollectionChanged)e.OldValue);
            ctrl.Subscribe((INotifyCollectionChanged)e.NewValue);
            ctrl.RebuildGrid();
        }

        private static void OnOrientationChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
            => ((SplitControl)d).RebuildGrid();

        private static void OnGridWidthChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
            => ((SplitControl)d).RebuildGrid();

        private void Subscribe(INotifyCollectionChanged col)
        {
            if (col != null) col.CollectionChanged += ItemsChanged;
        }

        private void Unsubscribe(INotifyCollectionChanged col)
        {
            if (col != null) col.CollectionChanged -= ItemsChanged;
        }

        private void ItemsChanged(object? sender, NotifyCollectionChangedEventArgs e)
            => RebuildGrid();

        private void RebuildGrid()
        {
            if (m_grid == null) return;

            m_grid.Children.Clear();
            m_grid.RowDefinitions.Clear();
            m_grid.ColumnDefinitions.Clear();

            if (ItemsSource == null) return;

            var items = new List<object>();
            foreach (var it in ItemsSource)
                items.Add(it);

            int n = items.Count;
            if (n == 0) return;

            // 列／行定義
            if (Orientation == Orientation.Horizontal)
            {
                for (int i = 0; i < n; i++)
                {
                    m_grid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(1, GridUnitType.Star),MinWidth=100 });
                }
            }
            else
            {
                for (int i = 0; i < n; i++)
                {
                    m_grid.RowDefinitions.Add(new RowDefinition { Height = new GridLength(1, GridUnitType.Star),MinHeight=100 });
                }
            }

            // 子要素の配置
            for (int i = 0; i < n; i++)
            {
                var content = new ContentControl
                {
                    Content = items[i],
                    ContentTemplateSelector = ContentTemplateSelector
                };
                if (Orientation == Orientation.Horizontal)
                    Grid.SetColumn(content, i);
                else
                    Grid.SetRow(content, i);

                m_grid.Children.Add(content);
            }

            // Splitter の配置
            for (int i = 1; i < n; i++)
            {
                var splitter = new GridSplitter
                {
                    ShowsPreview = true,
                };
                if (Orientation == Orientation.Horizontal)
                {
                    splitter.Width = GridWidth;
                    splitter.HorizontalAlignment = HorizontalAlignment.Left;
                    splitter.VerticalAlignment = VerticalAlignment.Stretch;
                    splitter.RenderTransform = new TranslateTransform(-GridWidth/2, 0);
                    Grid.SetColumn(splitter, i);
                }
                else
                {
                    splitter.Height = GridWidth;
                    splitter.HorizontalAlignment = HorizontalAlignment.Stretch;
                    splitter.VerticalAlignment = VerticalAlignment.Top;
                    splitter.ResizeDirection = GridResizeDirection.Rows;
                    splitter.RenderTransform = new TranslateTransform(0, -GridWidth/2);
                    Grid.SetRow(splitter, i);
                }

                m_grid.Children.Add(splitter);
            }
        }
    }
}

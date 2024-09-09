using System.Collections;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;

namespace CommonView.Controls
{
    [ContentProperty("Content")]
    public class InspectorView : Control
    {
        public static readonly DependencyProperty LabelWidthProperty = DependencyProperty.Register(
                nameof(LabelWidth),
                typeof(GridLength),
                typeof(InspectorView),
                new PropertyMetadata());

        //public static readonly DependencyProperty ItemsSourceProperty = DependencyProperty.Register(
        //        nameof(ItemsSource),
        //        typeof(IEnumerable),
        //        typeof(InspectorView),
        //        new PropertyMetadata());

        public static readonly DependencyProperty ContentProperty = DependencyProperty.Register(
                nameof(Content),
                typeof(object),
                typeof(InspectorView),
                new PropertyMetadata());

        public GridLength LabelWidth
        {
            get => (GridLength)GetValue(LabelWidthProperty);
            set => SetValue(LabelWidthProperty, value);
        }

        //public IEnumerable ItemsSource {
        //    get => (IEnumerable)GetValue(ItemsSourceProperty);
        //    set => SetValue(ItemsSourceProperty, value);
        //}
        public object? Content
        {
            get => GetValue(ContentProperty);
            set => SetValue(ContentProperty, value);
        }

        static InspectorView()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(InspectorView), new FrameworkPropertyMetadata(typeof(InspectorView)));
        }
    }
}

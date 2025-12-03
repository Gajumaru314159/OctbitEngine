using System.Windows;
using System.Windows.Controls;

namespace CommonView.Controls
{
    public class InspectorView : ItemsControl
    {
        public static readonly DependencyProperty IndentProperty = DependencyProperty.Register(
                nameof(Indent),
                typeof(double),
                typeof(InspectorView),
                new PropertyMetadata(0.0));

        public static readonly DependencyProperty IsExpandedProperty = DependencyProperty.Register(
                nameof(IsExpanded),
                typeof(bool),
                typeof(InspectorView),
                 new PropertyMetadata(true));

        public static readonly DependencyProperty LabelProperty = DependencyProperty.Register(
                nameof(Label),
                typeof(object),
                typeof(InspectorView),
                new PropertyMetadata());

        public static readonly DependencyProperty LabelWidthProperty = DependencyProperty.Register(
                nameof(LabelWidth),
                typeof(GridLength),
                typeof(InspectorView),
                new PropertyMetadata());

        public double Indent
        {
            get => (double)GetValue(IndentProperty);
            set => SetValue(IndentProperty, value);
        }

        public bool IsExpanded
        {
            get => (bool)GetValue(IsExpandedProperty);
            set => SetValue(IsExpandedProperty, value);
        }
        public object? Label
        {
            get => GetValue(LabelProperty);
            set => SetValue(LabelProperty, value);
        }

        public GridLength LabelWidth
        {
            get => (GridLength)GetValue(LabelWidthProperty);
            set => SetValue(LabelWidthProperty, value);
        }

        static InspectorView()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(InspectorView), new FrameworkPropertyMetadata(typeof(InspectorView)));
        }

    }
}

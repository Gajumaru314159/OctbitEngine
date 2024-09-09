using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;

namespace CommonView.Controls
{
    [ContentProperty("Content")]
    public class InspectorItemContainer : Control
    {
        static InspectorItemContainer()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(InspectorItemContainer), new FrameworkPropertyMetadata(typeof(InspectorItemContainer)));
        }

        public static readonly DependencyProperty LabelProperty = DependencyProperty.Register(
                nameof(Label),
                typeof(object),
                typeof(InspectorItemContainer),
                new PropertyMetadata());

        public static readonly DependencyProperty ContentProperty = DependencyProperty.Register(
                nameof(Content),
                typeof(object),
                typeof(InspectorItemContainer),
                new PropertyMetadata());

        public static readonly DependencyProperty LabelWidthProperty = DependencyProperty.Register(
                nameof(LabelWidth),
                typeof(GridLength),
                typeof(InspectorItemContainer),
                new PropertyMetadata());

        public static readonly DependencyProperty CollapseLabelProperty = DependencyProperty.Register(
                nameof(CollapseLabel),
                typeof(bool),
                typeof(InspectorItemContainer),
                new PropertyMetadata());


        public object? Label
        {
            get => GetValue(LabelProperty);
            set => SetValue(LabelProperty, value);
        }
        public object? Content
        {
            get => GetValue(ContentProperty);
            set => SetValue(ContentProperty, value);
        }
        public GridLength LabelWidth
        {
            get => (GridLength)GetValue(LabelWidthProperty);
            set => SetValue(LabelWidthProperty, value);
        }

        public bool CollapseLabel
        {
            get => (bool)GetValue(CollapseLabelProperty);
            set => SetValue(CollapseLabelProperty, value);
        }

    }
}

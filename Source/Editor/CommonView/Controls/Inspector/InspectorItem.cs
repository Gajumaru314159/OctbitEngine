using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;

namespace CommonView.Controls
{
    [ContentProperty("Content")]
    public class InspectorItem : Control
    {
        static InspectorItem()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(InspectorItem), new FrameworkPropertyMetadata(typeof(InspectorItem)));
        }

        public static readonly DependencyProperty IndentProperty = DependencyProperty.Register(
                nameof(Indent),
                typeof(double),
                typeof(InspectorItem),
                new PropertyMetadata(0.0));

        public static readonly DependencyProperty LabelProperty = DependencyProperty.Register(
                nameof(Label),
                typeof(object),
                typeof(InspectorItem),
                new PropertyMetadata());

        public static readonly DependencyProperty ContentProperty = DependencyProperty.Register(
                nameof(Content),
                typeof(object),
                typeof(InspectorItem),
                new PropertyMetadata());

        public static readonly DependencyProperty CollapseLabelProperty = DependencyProperty.Register(
                nameof(CollapseLabel),
                typeof(bool),
                typeof(InspectorItem),
                new PropertyMetadata());


        public double Indent
        {
            get => (double)GetValue(IndentProperty);
            set => SetValue(IndentProperty, value);
        }

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

        public bool CollapseLabel
        {
            get => (bool)GetValue(CollapseLabelProperty);
            set => SetValue(CollapseLabelProperty, value);
        }

    }
}

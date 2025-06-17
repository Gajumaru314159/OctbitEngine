using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;

namespace CommonView.Controls
{
    // ViewModel
    public class DockingBase
    {
        protected DockingBase()
        {
        }
    }

    public class DockingLayoutGroup : DockingBase
    {
        public ObservableCollection<DockingBase> Items { get; } = new();

        public ReactivePropertySlim<Orientation> Orientation { get; } = new(System.Windows.Controls.Orientation.Horizontal);

        public DockingLayoutGroup()
        {
        }

        public void Add(DockingBase item)
        {
            Items.Add(item);
        }
    }

    public class DockingTabGroup : DockingBase
    {
        public ObservableCollection<object> Items { get; } = new();

        public DockingTabGroup()
        {
        }

        public void Add(object item)
        {
            Items.Add(item);
        }
    }
    public class DockingTab
    {
        protected DockingTab(string title)
        {
            Title = title;
        }
        public string Title { get; private init; } = "Title";
    }


    // View
    public class DockingLayout : Control
    {
        static DockingLayout()
        {
            DefaultStyleKeyProperty.OverrideMetadata(
                typeof(DockingLayout),
                new FrameworkPropertyMetadata(typeof(DockingLayout)));
        }

        public static readonly DependencyProperty ItemProperty = DependencyProperty.Register(
                nameof(Item),
                typeof(DockingLayoutGroup),
                typeof(DockingLayout),
                new PropertyMetadata());

        public static readonly DependencyProperty ItemTemplateProperty = DependencyProperty.Register(
                nameof(ItemTemplate),
                typeof(DataTemplate),
                typeof(DockingLayout),
                new PropertyMetadata());

        public static readonly DependencyProperty ContentTemplateProperty = DependencyProperty.Register(
                nameof(ContentTemplate),
                typeof(DataTemplate),
                typeof(DockingLayout),
                new PropertyMetadata());


        public static readonly DependencyProperty ItemTemplateSelectorProperty = DependencyProperty.Register(
                nameof(ItemTemplateSelector),
                typeof(DataTemplateSelector),
                typeof(DockingLayout),
                new PropertyMetadata());

        public static readonly DependencyProperty ContentTemplateSelectorProperty = DependencyProperty.Register(
                nameof(ContentTemplateSelector),
                typeof(DataTemplateSelector),
                typeof(DockingLayout),
                new PropertyMetadata());
        public DockingLayoutGroup Item
        {
            get => (DockingLayoutGroup)GetValue(ItemProperty);
            set => SetValue(ItemProperty, value);
        }
        public DataTemplate ItemTemplate
        {
            get => (DataTemplate)GetValue(ItemTemplateProperty);
            set => SetValue(ItemTemplateProperty, value);
        }
        public DataTemplate ContentTemplate
        {
            get => (DataTemplate)GetValue(ContentTemplateProperty);
            set => SetValue(ContentTemplateProperty, value);
        }
        public DataTemplateSelector ItemTemplateSelector
        {
            get => (DataTemplateSelector)GetValue(ItemTemplateSelectorProperty);
            set => SetValue(ItemTemplateSelectorProperty, value);
        }
        public DataTemplateSelector ContentTemplateSelector
        {
            get => (DataTemplateSelector)GetValue(ContentTemplateSelectorProperty);
            set => SetValue(ContentTemplateSelectorProperty, value);
        }

    }


}

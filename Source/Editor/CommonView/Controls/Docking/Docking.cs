using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Windows;
using System.Windows.Controls;

namespace CommonView.Controls
{
    public class DockingBase
    {
        protected DockingBase() { }
    }

    public class DockingLayoutGroup : DockingBase
    {
        public ObservableCollection<DockingBase> Items { get; } = new();
        public ReactivePropertySlim<Orientation> Orientation { get; } = new(System.Windows.Controls.Orientation.Horizontal);

        // TabGroupごとのハンドラ登録管理
        private readonly Dictionary<DockingBase, NotifyCollectionChangedEventHandler> m_tabHandlers = new();

        public DockingLayoutGroup()
        {
            Items.CollectionChanged += OnItemsChanged;
        }

        public void Add(DockingBase item)
        {
            Items.Add(item);
        }

        private void OnItemsChanged(object? sender, NotifyCollectionChangedEventArgs e)
        {
            // 追加された要素をチェック
            if (e.Action == NotifyCollectionChangedAction.Add && e.NewItems != null)
            {
                foreach (var obj in e.NewItems)
                {
                    if (obj is DockingTabGroup tabGroup)
                    {
                        // 空になったら親から除去するハンドラ
                        NotifyCollectionChangedEventHandler handler = (s, args) =>
                        {
                            if (tabGroup.Items.Count == 0)
                            {
                                Items.Remove(tabGroup);
                            }
                        };

                        tabGroup.Items.CollectionChanged += handler;
                        m_tabHandlers[tabGroup] = handler;
                    }
                    if (obj is DockingLayoutGroup layoutGroup)
                    {
                        // 空になったら親から除去するハンドラ
                        NotifyCollectionChangedEventHandler handler = (s, args) =>
                        {
                            if (layoutGroup.Items.Count == 0)
                            {
                                Items.Remove(layoutGroup);
                            }
                        };

                        layoutGroup.Items.CollectionChanged += handler;
                        m_tabHandlers[layoutGroup] = handler;
                    }
                }
            }

            // 削除された要素はハンドラを解除
            if (e.Action == NotifyCollectionChangedAction.Remove && e.OldItems != null)
            {
                foreach (var obj in e.OldItems)
                {
                    if (obj is DockingTabGroup tabGroup && m_tabHandlers.TryGetValue(tabGroup, out var handler1))
                    {
                        tabGroup.Items.CollectionChanged -= handler1;
                        m_tabHandlers.Remove(tabGroup);
                    }
                    if (obj is DockingLayoutGroup layoutGroup && m_tabHandlers.TryGetValue(layoutGroup, out var handler2))
                    {
                        layoutGroup.Items.CollectionChanged -= handler2;
                        m_tabHandlers.Remove(layoutGroup);
                    }
                }
            }

            // Reset（Clear）時は全ハンドラを解除
            if (e.Action == NotifyCollectionChangedAction.Reset)
            {
                foreach (var kv in m_tabHandlers)
                {
                    if(kv.Key is DockingTabGroup tabGroup) tabGroup.Items.CollectionChanged -= kv.Value;
                    if(kv.Key is DockingLayoutGroup layoutGroup) layoutGroup.Items.CollectionChanged -= kv.Value;
                }
                m_tabHandlers.Clear();
            }
        }

    }

    public class DockingTabGroup : DockingBase
    {
        public ObservableCollection<object> Items { get; } = new();

        public DockingTabGroup() { }

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

using Common.Tree;
using CommonView.Controls;
using CommonView.Menu;
using Reactive.Bindings;
using System.Windows;
using System.Windows.Input;

namespace OctbitEditor
{
    /// <summary>
    /// Interaction logic for DockingTest.xaml
    /// </summary>
    public partial class DockingTest : Window
    {
        public DockingTest()
            : this(false)
        {
        }
        public DockingTest(bool empty)
        {
            var vm = new DockingTestVM(empty);
            DataContext = vm;
            InitializeComponent();
        }

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            if(sender is FrameworkElement f)
            {
                
            }
        }
    }

    public class DockingTab
    {
        public DockingTab()
        {
        }
        public DockingTab(string title)
        {
            Title = title;
        }
        public string Title { get; init; } = "Title";
    }
    public class DockingTabA : DockingTab
    {
        public DockingTabA()
            : base("Tab A")
        {
        }
    }
    public class DockingTabB : DockingTab
    {
        public DockingTabB()
            : base("Tab B")
        {
        }
    }

    public class DockingTabC : DockingTab
    {
        public DockingTabC()
            : base("Tab C")
        {
        }
    }

    public class DockingTestVM
    {
        public DockingTestVM()
            : this(false)
        { }

        public DockingTestVM(bool empty)
        {
            if (!empty)
            {
                var tab1 = new DockingTabGroup();
                tab1.Add(new DockingTabA());
                tab1.Add(new DockingTabB());
                tab1.Add(new DockingTabC());
                Item.Add(tab1);


                var tab2 = new DockingTabGroup();
                tab2.Add(new DockingTabB());
                Item.Add(tab2);

                var tab3l = new DockingLayoutGroup();
                tab3l.Orientation.Value = System.Windows.Controls.Orientation.Vertical;
                var tab3_1 = new DockingTabGroup();
                tab3_1.Add(new DockingTabC());
                tab3_1.Add(new DockingTabB());
                tab3l.Add(tab3_1);
                var tab3_2 = new DockingTabGroup();
                tab3_2.Add(new DockingTabA());
                tab3_2.Add(new DockingTabB());
                tab3l.Add(tab3_2);
                Item.Add(tab3l);
            }

            AddTabCommand = new DelegateCommand(() =>
            {
                var newTab = new DockingTabGroup();
                newTab.Add(new DockingTabA());
                newTab.Add(new DockingTabB());
                newTab.Add(new DockingTabC());
                Item.Add(newTab);
            });


            CloseCommand = new DelegateCommand(vm =>
            {
                if (vm == null) return;

                DockingTabGroup? parent = FindTabGroup(i => i.Items.Contains(vm));

                if (parent != null)
                {
                    parent.Items.Remove(vm);
                }
            });

            WindowCommand = new DelegateCommand(vm =>
            {
                if (vm == null) return;

                DockingTabGroup? parent = FindTabGroup(i=>i.Items.Contains(vm));

                if (parent!=null)
                {
                    parent.Items.Remove(vm);

                    var tab = new DockingTabGroup();
                    tab.Add(vm);

                    var newvm = new DockingTestVM(true);
                    newvm.Item.Add(tab);

                    var window = new DockingTest(true) { DataContext = newvm };
                    window.Show();
                }
            });
        }
        private DockingTabGroup? FindTabGroup(Predicate<DockingTabGroup> func)
        {
            DockingTabGroup? result = null;
            VisitTabGroup(tabs =>
            {
                if (func(tabs))
                {
                    result = tabs;
                }
            });
            return result;
        }

        private void VisitTabGroup(Action<DockingTabGroup> func)
        {
            void visit(DockingLayoutGroup g)
            {
                foreach (var item in g.Items)
                {
                    if (item is DockingTabGroup tabGroup)
                    {
                        func(tabGroup);
                    }
                    if (item is DockingLayoutGroup layoutGroup)
                    {
                        visit(layoutGroup);
                    }
                }

            }
            visit(Item);
        }

        public DockingLayoutGroup Item { get; } = new DockingLayoutGroup();
        public ICommand AddTabCommand { get; }
        public ICommand CloseCommand { get; }
        public ICommand WindowCommand { get; }
    }

}

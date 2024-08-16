using System.Windows;
using UserControl = System.Windows.Controls.UserControl;

namespace OctbitEditor
{
    /// <summary>
    /// Explorer.xaml の相互作用ロジック
    /// </summary>
    public partial class Explorer : UserControl
    {
        public Explorer()
        {
            InitializeComponent();
        }

        private void TreeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (DataContext is not ExplorerVM vm) return;
            if(e.NewValue is ExplorerItem item)
            {
                vm.SelectedFolder.Value = item;
            }
        }
    }
}

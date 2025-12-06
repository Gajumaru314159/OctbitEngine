using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace OctbitEditor
{
    /// <summary>
    /// Outliner.xaml の相互作用ロジック
    /// </summary>
    public partial class Outliner : UserControl
    {
        public Outliner()
        {
            InitializeComponent();
        }

        private void OnDataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            // XAML上でMultiSelectTreeView.SelectedItemsをバインディングするとNullReferenceExceptionが発生するためコード上でバインディング
            _tree.DataContext = e.NewValue;
            _tree.SetBinding(MultiSelectTreeView.SelectedItemsProperty, nameof(OutlinerVM.SelectedItems));

        }

        private void OnPreviewKeyDown(object sender, KeyEventArgs e)
        {
            // F2キーの入力がMultiSelectTreeViewで処理されてしまうためPreviewKeyDownで処理
            // MultiSelectTreeViewを継続して使用するか確定していないためMultiSelectTreeViewのEdit機能は使用しない
            if (sender is not Outliner outliner) return;
            if (outliner.DataContext is not OutlinerVM vm) return;
            if (e.Key == Key.F2)
            {
                vm.EditEntityName();
                e.Handled = true;
            }
        }

        private void Root_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Delete)
            {
                _tree.Focus();
                if (DataContext is OutlinerVM vm)
                {
                    vm.DeleteEntityCommand?.Execute(null);
                }
                e.Handled=true;
            }
        }
    }
}

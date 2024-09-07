using System.Windows.Controls;
using System.Windows.Input;
using UserControl = System.Windows.Controls.UserControl;

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

        private void Root_MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            if (Keyboard.FocusedElement is System.Windows.Controls.TextBox textBox)
            {
                TraversalRequest tRequest = new TraversalRequest(FocusNavigationDirection.Next);
                textBox.MoveFocus(tRequest);
            }
        }

        private void Root_DataContextChanged(object sender, System.Windows.DependencyPropertyChangedEventArgs e)
        {
            // XAML上でMultiSelectTreeView.SelectedItemsをバインディングするとNullReferenceExceptionが発生するためコード上でバインディング
            _tree.DataContext = e.NewValue;
            _tree.SetBinding(MultiSelectTreeView.SelectedItemsProperty, nameof(OutlinerVM.SelectedItems));
        }
    }
}

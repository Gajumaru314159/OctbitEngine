using System.Windows.Controls;
using System.Windows.Input;
using System.Windows;

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

        private void Root_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (Keyboard.FocusedElement is TextBox textBox)
            {
                TraversalRequest tRequest = new TraversalRequest(FocusNavigationDirection.Next);
                textBox.MoveFocus(tRequest);
            }
        }

        private void Root_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            // XAML上でMultiSelectTreeView.SelectedItemsをバインディングするとNullReferenceExceptionが発生するためコード上でバインディング
            _tree.DataContext = e.NewValue;
            _tree.SetBinding(MultiSelectTreeView.SelectedItemsProperty, nameof(OutlinerVM.SelectedItems));
        }
    }
}

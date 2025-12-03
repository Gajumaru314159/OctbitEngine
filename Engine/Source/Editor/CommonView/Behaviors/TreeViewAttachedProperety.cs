using System.Windows;
using System.Windows.Controls;

namespace CommonView
{
    public static class TreeViewEx
    {
        public static readonly DependencyProperty SelectedItemProperty =
            DependencyProperty.RegisterAttached(
                "SelectedItem",
                typeof(object),
                typeof(TreeViewEx),
                new PropertyMetadata(null, OnSelectedItemChanged));

        public static object GetSelectedItem(DependencyObject obj)
        {
            return obj.GetValue(SelectedItemProperty);
        }

        public static void SetSelectedItem(DependencyObject obj, object value)
        {
            obj.SetValue(SelectedItemProperty, value);
        }

        private static void OnSelectedItemChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is TreeView treeView)
            {
                treeView.SelectedItemChanged -= TreeView_SelectedItemChanged;
                treeView.SelectedItemChanged += TreeView_SelectedItemChanged;
            }
        }

        private static void TreeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (sender is TreeView treeView)
            {
                SetSelectedItem(treeView, e.NewValue);
            }
        }
    }
}

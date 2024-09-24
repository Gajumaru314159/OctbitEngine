using System.Windows;
using System.Windows.Controls;

namespace OctbitEditor
{
    /// <summary>
    /// AssetBrowser.xaml の相互作用ロジック
    /// </summary>
    public partial class AssetBrowser : UserControl
    {
        AssetBrowserVM? ViewModel => DataContext as AssetBrowserVM;
        public AssetBrowser()
        {
            InitializeComponent();
        }

        private void Grid_Drop(object sender, DragEventArgs e)
        {
            if (ViewModel is null) return;
            var list = ToPathArray(e).Where(i => ViewModel.AssetManager.CanImport(i)).ToArray();
            foreach (var path in list)
            {
                ViewModel.AssetManager.RootFolder.Import(path);
            }
        }

        private void ListBox_DragOver(object sender, DragEventArgs e)
        {
            e.Effects = CanImport(e) ? DragDropEffects.Copy : DragDropEffects.None;
            e.Handled = true;
        }

        private string[] ToPathArray(DragEventArgs e)
        {
            if (!e.Data.GetDataPresent(DataFormats.FileDrop)) return Array.Empty<string>();
            return e.Data.GetData(DataFormats.FileDrop) as string[] ?? Array.Empty<string>();
        }
        private bool CanImport(DragEventArgs e)
        {
            if (ViewModel is null) return false;
            return ToPathArray(e).Any(i => ViewModel.AssetManager.CanImport(i));
        }

        private void UserControl_DataContextChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            // XAML上でMultiSelectTreeView.SelectedItemsをバインディングするとNullReferenceExceptionが発生するためコード上でバインディング
            _tree.DataContext = e.NewValue;
            _tree.SetBinding(MultiSelectTreeView.SelectedItemsProperty, nameof(AssetBrowserVM.SelectedItems));
        }
    }
}

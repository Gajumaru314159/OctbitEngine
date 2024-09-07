using Common.Log;
using OctbitEngine.Asset;
using System.Windows.Controls;
using DataFormats = System.Windows.DataFormats;
using DragDropEffects = System.Windows.DragDropEffects;
using UserControl = System.Windows.Controls.UserControl;

namespace OctbitEditor
{
    /// <summary>
    /// Explorer.xaml の相互作用ロジック
    /// </summary>
    public partial class Explorer : UserControl
    {
        ExplorerVM? ViewModel => DataContext as ExplorerVM;
        public Explorer()
        {
            InitializeComponent();
        }

        private void Grid_Drop(object sender, System.Windows.DragEventArgs e)
        {
            if (ViewModel is null) return;
            var list = ToPathArray(e).Where(i => ViewModel.AssetManager.CanImport(i)).ToArray();
            foreach (var path in list)
            {
                ViewModel.AssetManager.RootFolder.Import(path);
            }
        }

        private void ListBox_DragOver(object sender, System.Windows.DragEventArgs e)
        {
            e.Effects = CanImport(e) ? DragDropEffects.Copy : DragDropEffects.None;
            e.Handled = true;
        }

        private string[] ToPathArray(System.Windows.DragEventArgs e)
        {
            if (!e.Data.GetDataPresent(DataFormats.FileDrop)) return Array.Empty<string>();
            return e.Data.GetData(DataFormats.FileDrop) as string[] ?? Array.Empty<string>();
        }
        private bool CanImport(System.Windows.DragEventArgs e)
        {
            if (ViewModel is null) return false;
            return ToPathArray(e).Any(i => ViewModel.AssetManager.CanImport(i));
        }

        private void UserControl_DataContextChanged(object sender, System.Windows.DependencyPropertyChangedEventArgs e)
        {
            // XAML上でMultiSelectTreeView.SelectedItemsをバインディングするとNullReferenceExceptionが発生するためコード上でバインディング
            _tree.DataContext = e.NewValue;
            _tree.SetBinding(MultiSelectTreeView.SelectedItemsProperty, nameof(ExplorerVM.SelectedItems));
        }
    }
}

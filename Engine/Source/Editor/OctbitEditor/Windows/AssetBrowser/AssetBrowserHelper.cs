using Common.Generic;
using Common.Linq;
using Common.Log;
using Common.Tree;
using CommonView.Menu;
using Livet;
using OctbitEngine.Asset;
using OctbitEngine.Config;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;
using System.Collections.ObjectModel;
using System.IO;
using System.Reactive.Linq;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Collections.Specialized;
using CommonView.Controls;

namespace OctbitEditor
{

    public class AssetBrowserDetailItemDataSelector : DataTemplateSelector
    {
        static AssetBrowserDetailItemDataSelector()
        {

        }

        public override DataTemplate? SelectTemplate(object item, DependencyObject container)
        {
            if (container is not FrameworkElement f) return null;
            if (item is AssetBrowserFolderItem folder)
            {
                return f.FindResource("InspectablePropertyTemplate") as DataTemplate;
            }
            if (item is AssetBrowserFileItem file)
            {
                return f.FindResource("InspectablePropertyTemplate") as DataTemplate;
            }
            if (item is AssetBrowserAssetItem asset)
            {
                return f.FindResource("InspectablePropertyTemplate") as DataTemplate;
            }
            return null;
        }

    }

    class ColumnViewModel : ViewModel
    {
        public string Header { get; } = string.Empty;
        public string? DisplayMemberPath { get; }
        public ICommand? SortCommand { get; }
    }


    class AssetBrowserHelper
    {
        public static readonly DependencyProperty DynamicColumnsProperty =
           DependencyProperty.RegisterAttached("DynamicColumns", typeof(ObservableCollection<ColumnViewModel>), typeof(AssetBrowserHelper), new PropertyMetadata(null, DynamicColumnsChanged));

        private static void DynamicColumnsChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            if (d is not GridView gridView) return;
            if (e.NewValue is not ObservableCollection<ColumnViewModel> columnVms) return;

            gridView.Columns.Clear();
            foreach (var columnVm in columnVms)
            {
                gridView.Columns.Add(ToGridViewColumn(columnVm));
            }
            columnVms.CollectionChanged += (_, e) =>
            {
                switch (e.Action)
                {
                    case NotifyCollectionChangedAction.Add:
                        var added = (ColumnViewModel)e.NewItems![0]!;
                        gridView.Columns.Add(ToGridViewColumn(added));
                        break;
                    case NotifyCollectionChangedAction.Remove:
                        var removed = e.OldStartingIndex;
                        gridView.Columns.Remove(gridView.Columns[removed]);
                        break;
                    case NotifyCollectionChangedAction.Replace:
                        var replace = e.NewStartingIndex;
                        gridView.Columns[replace] = ToGridViewColumn((ColumnViewModel)e.NewItems![0]!);
                        break;
                    case NotifyCollectionChangedAction.Move:
                        gridView.Columns.Move(e.OldStartingIndex, e.NewStartingIndex);
                        break;
                    case NotifyCollectionChangedAction.Reset:
                        gridView.Columns.Clear();
                        break;
                }
            };
        }

        private static GridViewColumn ToGridViewColumn(ColumnViewModel columnVM)
        {
            return new GridViewColumn
            {
                Header = columnVM,
                // HeaderTemplate
                // Width
                CellTemplate = null,// Attribute?DisplayMemberPath?
                // HeaderTemplateSelector
                // CellTemplateSelector
                DisplayMemberBinding = columnVM.DisplayMemberPath != null ? new Binding(columnVM.DisplayMemberPath):null,
            };
        }
    }
}

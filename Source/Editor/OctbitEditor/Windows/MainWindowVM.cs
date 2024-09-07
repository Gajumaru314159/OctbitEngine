using CommonView.History;
using CommonView.Menu;
using Livet;
using OctbitEngine.Asset;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.Windows.Input;

namespace OctbitEditor
{
    internal class MainWindowVM : ViewModel
    {
        public MainWindowVM()
        {
            Title.Value = "Octbit Editor";

            ExitCommand = new DelegateCommand(_ => System.Windows.Application.Current.Shutdown());

            RedoCommand = new DelegateCommand(() => History.Redo());
            UndoCommand = new DelegateCommand(() =>
            {
                History.Undo();
            });

            History.Executed += () =>
            {
                RaisePropertyChanged(nameof(CanRedo));
                RaisePropertyChanged(nameof(CanUndo));
            };

            var assetManager = AssetManager.Instance;

            _dockingDocumentViewModels.Add(new ViewportVM());
            _dockingPaneViewModels.Add(new ExplorerVM(assetManager));
            _dockingPaneViewModels.Add(new HistoryWindowVM());
            _dockingPaneViewModels.Add(new LogListVM());
            _dockingPaneViewModels.Add(new OutlinerVM());
        }

        public ReadOnlyObservableCollection<ViewModel> DockingDocumentViewModels => new(_dockingDocumentViewModels);
        private ObservableCollection<ViewModel> _dockingDocumentViewModels = new();
        public ReadOnlyObservableCollection<ViewModel> DockingPaneViewModels => new(_dockingPaneViewModels);
        private ObservableCollection<ViewModel> _dockingPaneViewModels = new();


        public ReactivePropertySlim<string> Title { get; } = new();

        public ICommand RedoCommand { get; }
        public ICommand UndoCommand { get; }

        public bool CanRedo => History.CanRedo;
        public bool CanUndo => History.CanUndo;

        public ICommand ExitCommand { get; }
    }
}

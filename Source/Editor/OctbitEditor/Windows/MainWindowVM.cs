using System;
using System.Windows;
using Reactive.Bindings;
using CommonView.History;
using Livet;
using System.Windows.Input;
using CommonView.Menu;
using System.Collections.ObjectModel;
using OctbitEditor.Windows;
using OctbitEngine.Asset;

namespace OctbitEditor
{
    internal class MainWindowVM : ViewModel
    {
        public MainWindowVM()
        {
            Title.Value = "Octbit Editor";

            ExitCommand.Subscribe(_ => System.Windows.Application.Current.Shutdown());

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


        public ReactiveProperty<string> Title { get; } = new();

        public ICommand RedoCommand { get; }
        public ICommand UndoCommand { get; }

        public bool CanRedo => History.CanRedo;
        public bool CanUndo => History.CanUndo;

        public ReactiveCommand ExitCommand { get; } = new();
    }
}

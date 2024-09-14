using CommonView.History;
using CommonView.Menu;
using Livet;
using OctbitEngine.Asset;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.Windows.Input;

namespace OctbitEditor
{
    public enum ThemePreset
    {
        Dark,
        Light,
        UnityDark,
        UnityLight,
        UnrealEngine,
        Cyber,
        Maya,
        VisualStudioDark,
        Deep,
        //Modo,
        //Cyber,
    }
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
            _dockingPaneViewModels.Add(new AssetBrowserVM(assetManager));
            _dockingPaneViewModels.Add(new HistoryWindowVM());
            _dockingPaneViewModels.Add(new LogListVM());
            _dockingPaneViewModels.Add(new OutlinerVM());
        }

        public ReadOnlyObservableCollection<ViewModel> DockingDocumentViewModels => new(_dockingDocumentViewModels);
        private ObservableCollection<ViewModel> _dockingDocumentViewModels = new();
        public ReadOnlyObservableCollection<ViewModel> DockingPaneViewModels => new(_dockingPaneViewModels);
        private ObservableCollection<ViewModel> _dockingPaneViewModels = new();


        public ReactivePropertySlim<string> Title { get; } = new();

        public ThemePreset ThemePreset { get; set; } = OctbitEditor.ThemePreset.Dark;
        public IEnumerable<ThemePreset> ThemePresets { get; private set; } = Enum.GetValues<ThemePreset>();

        public ICommand RedoCommand { get; }
        public ICommand UndoCommand { get; }

        public bool CanRedo => History.CanRedo;
        public bool CanUndo => History.CanUndo;

        public ICommand ExitCommand { get; }
    }
}

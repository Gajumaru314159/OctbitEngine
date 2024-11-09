using Common.Log;
using CommonView.History;
using CommonView.Menu;
using Livet;
using OctbitEngine;
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
        Warm,
        Blue,
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

            Log.BeginShink();

            _coreSystem = new CoreSystem();

            _dockingDocumentViewModels.Add(new ViewportVM(_coreSystem.Runtime));
            _dockingPaneViewModels.Add(new OutlinerVM(_coreSystem.Runtime.ViewportWorld));
            _dockingPaneViewModels.Add(new InspectorVM());
            _dockingPaneViewModels.Add(new HistoryWindowVM());
            _dockingPaneViewModels.Add(new AssetBrowserVM(_coreSystem.AssetManager));
            _dockingPaneViewModels.Add(new LogListVM());

            Log.EndShink();

            Title.Value = "Octbit Editor";

            ExitCommand = new DelegateCommand(_ => System.Windows.Application.Current.Shutdown());

            RedoCommand = new DelegateCommand(() => History.Redo());
            UndoCommand = new DelegateCommand(() =>
            {
                History.Undo();
            });

            NewSceneCommand = new DelegateCommand(() =>
            {
                _coreSystem.Runtime.ViewportWorld.CreateScene();
            });

            History.Executed += () =>
            {
                RaisePropertyChanged(nameof(CanRedo));
                RaisePropertyChanged(nameof(CanUndo));
            };
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

        public ICommand NewSceneCommand { get; }

        public bool CanRedo => History.CanRedo;
        public bool CanUndo => History.CanUndo;

        public ICommand ExitCommand { get; }

        private CoreSystem _coreSystem;
    }
}

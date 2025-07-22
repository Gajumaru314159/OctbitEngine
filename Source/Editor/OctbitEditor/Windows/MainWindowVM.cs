using Common.Log;
using CommonView.Controls;
using CommonView.History;
using CommonView.Menu;
using Livet;
using OctbitEngine;
using OctbitEngine.Asset;
using Reactive.Bindings;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;
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

#if false // 独自ドッキング
                Item.Orientation.Value = Orientation.Vertical;
                var t = new DockingLayoutGroup(Orientation.Horizontal);
                t.AddTab(new OutlinerVM(_coreSystem.Runtime.ViewportWorld));
                t.AddTab(new ViewportVM(_coreSystem.Runtime));
                t.AddTab(new InspectorVM(_coreSystem.Runtime));
                var b = new DockingLayoutGroup(Orientation.Horizontal);
                b.AddTab(new AssetBrowserVM(_coreSystem.AssetManager));
                b.AddTab(new LogListVM(), new HistoryWindowVM());
#else
                _dockingDocumentViewModels.Add(new ViewportVM(_coreSystem.Runtime));
                _dockingPaneViewModels.Add(new OutlinerVM(_coreSystem.Runtime.ViewportWorld));
                _dockingPaneViewModels.Add(new InspectorVM(_coreSystem.Runtime));
                _dockingPaneViewModels.Add(new HistoryWindowVM());
                _dockingPaneViewModels.Add(new AssetBrowserVM(_coreSystem.AssetManager));
                _dockingPaneViewModels.Add(new LogListVM());
#endif

            Log.EndShink();

            GenerateMenu();

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
                CanUndo.Value = History.CanUndo;
                CanRedo.Value = History.CanRedo;
            };

        }

        private void GenerateMenu()
        {
            using(var group = MenuItems.AddGroup("_File"))
            {
                group.AddEmptyCommand("_New Scene","Ctrl+N");
                group.AddEmptyCommand("_Open Scene","Ctrl+O");
                group.AddSeparator();
                group.AddEmptyCommand("_Save", "Ctrl+S");
                group.AddEmptyCommand("Save _As...", "Ctrl+Shift+S");
                group.AddSeparator();
                group.AddEmptyCommand("New Project...");
                group.AddEmptyCommand("Open Project");
                group.AddEmptyCommand("Save Project");
                group.AddSeparator();
                group.AddCommand("_Exit", Application.Current.Shutdown);
            }
            using(var group = MenuItems.AddGroup("_Edit"))
            {
                group.AddCommand("_Undo","Ctrl+Z", History.Undo, CanUndo);
                group.AddCommand("_Redo","Ctrl+Y", History.Redo,CanRedo);
                group.AddSeparator();
                group.AddEmptyCommand("Select All","Ctrl+A");
                group.AddEmptyCommand("Deselect All","Ctrl+Shift+A");
                group.AddEmptyCommand("Select Children","Shift+C");
                group.AddEmptyCommand("Select Prefab Root","Ctrl+Shift+R");
                group.AddSeparator();
                group.AddEmptyCommand("_Cut", "Ctrl+X");
                group.AddEmptyCommand("_Copy", "Ctrl+C");
                group.AddEmptyCommand("_Paste", "Ctrl+V");
                group.AddSeparator();
                group.AddEmptyCommand("Duplicate", "Ctrl+D");
                group.AddEmptyCommand("Rename", "F2");
                group.AddEmptyCommand("Delete", "Delete");
                group.AddSeparator();
                group.AddEmptyCommand("Find","Ctrl+F");
                group.AddSeparator();
                group.AddEmptyCommand("Play","Ctrl+P");
                group.AddEmptyCommand("Pause","Ctrl+Shift+P");
                group.AddEmptyCommand("Step", "Ctrl+Alt+P");
                group.AddSeparator();
                group.AddEmptyCommand("Editor Config");
                group.AddEmptyCommand("Runtime Config");
            }

            using (var group = MenuItems.AddGroup("_Assets"))
            {
                group.AddEmptyCommand("New");
            }
            using (var group = MenuItems.AddGroup("_GameObject"))
            {
                group.AddEmptyCommand("New");
            }
            using (var group = MenuItems.AddGroup("_Component"))
            {
                group.AddEmptyCommand("New");
            }
            using (var group = MenuItems.AddGroup("_Window"))
            {
                group.AddCommand("_Outliner", () => { _dockingPaneViewModels.Add(new OutlinerVM(_coreSystem.Runtime.ViewportWorld)); });
                group.AddCommand("_Inspector", () => { _dockingPaneViewModels.Add(new InspectorVM(_coreSystem.Runtime)); });
                group.AddCommand("_History", () => { _dockingPaneViewModels.Add(new HistoryWindowVM()); });
                group.AddCommand("_AssetBrowser", () => { _dockingPaneViewModels.Add(new AssetBrowserVM(_coreSystem.AssetManager)); });
                group.AddCommand("_Log", () => { _dockingPaneViewModels.Add(new LogListVM()); });
            }
            using (var group = MenuItems.AddGroup("_Help"))
            {
                group.AddEmptyCommand("About");
                group.AddEmptyCommand("Manual");
                group.AddEmptyCommand("API Refelence");
                group.AddEmptyCommand("Release Notes");
            }


        }

        public void Clear()
        {
            _dockingDocumentViewModels.Clear();
            _dockingPaneViewModels.Clear();
        }

        public ReadOnlyObservableCollection<ViewModel> DockingDocumentViewModels => new(_dockingDocumentViewModels);
        private ObservableCollection<ViewModel> _dockingDocumentViewModels = new();
        public ReadOnlyObservableCollection<ViewModel> DockingPaneViewModels => new(_dockingPaneViewModels);
        private ObservableCollection<ViewModel> _dockingPaneViewModels = new();


        public DockingLayoutGroup Item { get; } = new DockingLayoutGroup();


        public ReactivePropertySlim<string> Title { get; } = new();

        public DynamicGroupItem MenuItems { get; } = new("Root");

        public ThemePreset ThemePreset { get; set; } = OctbitEditor.ThemePreset.Dark;
        public IEnumerable<ThemePreset> ThemePresets { get; private set; } = Enum.GetValues<ThemePreset>();

        public ICommand RedoCommand { get; }
        public ICommand UndoCommand { get; }

        public ICommand NewSceneCommand { get; }

        public ReactiveProperty<bool> CanRedo { get; } = new();
        public ReactiveProperty<bool> CanUndo { get; } = new();

        public ICommand ExitCommand { get; }

        private CoreSystem _coreSystem;
    }
}

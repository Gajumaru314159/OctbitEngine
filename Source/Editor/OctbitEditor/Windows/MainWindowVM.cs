using System;
using System.Windows;
using Reactive.Bindings;
using CommonView.History;
using Livet;
using System.Windows.Input;
using CommonView.Menu;

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
        }

        public ReactiveProperty<string> Title { get; } = new();

        public ICommand RedoCommand { get; }
        public ICommand UndoCommand { get; }

        public bool CanRedo => History.CanRedo;
        public bool CanUndo => History.CanUndo;

        public ReactiveCommand ExitCommand { get; } = new();
    }
}

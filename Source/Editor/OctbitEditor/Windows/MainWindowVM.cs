using System.Windows;
using Reactive.Bindings;

namespace OctbitEditor
{
    internal class MainWindowVM
    {
        public MainWindowVM()
        {
            Title.Value = "Octbit Editor";

            ExitCommand.Subscribe(_ => System.Windows.Application.Current.Shutdown());
        }

        public ReactiveProperty<string> Title { get; } = new();

        public ReactiveCommand ExitCommand { get; } = new();
    }
}

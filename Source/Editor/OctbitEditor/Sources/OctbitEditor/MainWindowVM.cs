using System.Windows;
using Reactive.Bindings;

namespace OctbitEditor
{
    internal class MainWindowVM
    {
        public MainWindowVM()
        {
            Message.Value = "Hello, World!";

            ExitCommand.Subscribe(_ => System.Windows.Application.Current.Shutdown());
        }

        public ReactiveProperty<string> Message { get; } = new();

        public ReactiveCommand ExitCommand { get; } = new();
    }
}

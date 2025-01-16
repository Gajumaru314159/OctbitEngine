using Reactive.Bindings;
using System.Windows.Input;
using System.Windows.Media;

namespace CommonView.Menu
{

    public class DynamicMenuItem
    {
        public bool IsSeparator { get; protected set; }
        public string? Header { get; protected set; }
        public string? Gesture { get; protected set; }
        public List<DynamicMenuItem> Children { get; } = new();
        public ICommand? Command { get; protected set; }
        public ImageSource? Icon { get; set; }
    }

    public class DynamicGroupItem: DynamicMenuItem,IDisposable
    {
        public DynamicGroupItem(string header)
        {
            Header = header;
        }
        public DynamicCommandItem AddEmptyCommand(string header, string? gesture = null)
        {
            var item = new DynamicCommandItem(header,gesture, () => { });
            Children.Add(item);
            return item;
        }
        public DynamicCommandItem AddCommand(string header, Action action, IObservable<bool>? canExecute = null)
        {
            var item = new DynamicCommandItem(header,action,canExecute);
            Children.Add(item);
            return item;
        }
        public DynamicCommandItem AddCommand(string header, string gesture, Action action, IObservable<bool>? canExecute = null)
        {
            var item = new DynamicCommandItem(header,gesture,action,canExecute);
            Children.Add(item);
            return item;
        }
        public DynamicGroupItem AddGroup(string header)
        {
            var item = new DynamicGroupItem(header);
            Children.Add(item);
            return item;
        }
        public DynamicSeparatorItem AddSeparator()
        {
            var item = new DynamicSeparatorItem();
            Children.Add(item);
            return item;
        }

        protected virtual void Dispose(bool disposing)
        {
            // Disposeすべきリソースは持たないが、グルーピング時にusingを使うために実装。
        }

        public void Dispose()
        {
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }
    public class DynamicCommandItem : DynamicMenuItem
    {
        public DynamicCommandItem(string header, Action action, IObservable<bool>? canExecute = null)
            : this(header,null,action,canExecute)
        {
        }
        public DynamicCommandItem(string header, string? gesture, Action action, IObservable<bool>? canExecute = null)
        {
            Header = header;
            Gesture = gesture;
            if (canExecute is null)
                Command =  new DelegateCommand(action);
            else
                Command = canExecute.ToReactiveCommandSlim();
        }
    }
    public class DynamicSeparatorItem : DynamicMenuItem
    {
        public DynamicSeparatorItem()
        {
            IsSeparator = true;
        }
    }

}

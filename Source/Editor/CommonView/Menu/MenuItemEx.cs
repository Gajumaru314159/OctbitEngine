using Reactive.Bindings;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
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

    public class DynamicGroupItem: DynamicMenuItem
    {
        public DynamicGroupItem(string header)
        {
            Header = header;
        }
        public DynamicCommandItem AddCommand(string header, Action action, Func<object?, bool>? canExecute = null)
        {
            var item = new DynamicCommandItem(header,action,canExecute);
            Children.Add(item);
            return item;
        }
        public DynamicCommandItem AddCommand(string header, string gesture, Action action, Func<object?, bool>? canExecute = null)
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

    }
    public class DynamicCommandItem : DynamicMenuItem
    {
        public DynamicCommandItem(string header, Action action, Func<object?, bool>? canExecute = null)
        {
            Header = header;
            Gesture = string.Empty;
            Command = new DelegateCommand(action, canExecute);
        }
        public DynamicCommandItem(string header, string gesture, Action action, Func<object?, bool>? canExecute = null)
        {
            Header = header;
            Gesture = gesture;
            Command = new DelegateCommand(action, canExecute);
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

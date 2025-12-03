using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace CommonView.Menu
{
    public class DelegateCommand : ICommand
    {

        public DelegateCommand(Action<object?> execute, Func<object?, bool>? canExecute = null)
        {
            m_execute = execute ?? throw new ArgumentNullException(nameof(execute));
            m_canExecute = canExecute;

            CanExecuteChanged?.Invoke(null,new());
        }
        public DelegateCommand(Action execute, Func<object?, bool>? canExecute = null)
            : this(_=>execute.Invoke(), canExecute)
        {
        }

        public event EventHandler? CanExecuteChanged;

        public bool CanExecute(object? parameter)
        {
            return m_canExecute?.Invoke(parameter) ?? true;
        }

        public void Execute(object? parameter)
        {
            if (CanExecute(parameter))
            {
                m_execute(parameter);
            }
        }
        private readonly Action<object?> m_execute;
        private readonly Func<object?, bool>? m_canExecute;

    }
}

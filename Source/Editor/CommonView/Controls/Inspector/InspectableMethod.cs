using System.Windows.Input;

namespace CommonView.Controls
{
    public abstract class InspectableMethod : Inspectable
    {
        protected InspectableMethod()
        {

        }

        public abstract string DisplayName { get; }
        public abstract string Name { get; }
        public abstract ICommand Command { get; }
        public abstract IReadOnlyDictionary<string, string> Tags { get; }
    }

}

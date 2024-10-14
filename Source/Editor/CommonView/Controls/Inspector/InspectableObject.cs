using System.Collections.ObjectModel;

namespace CommonView.Controls
{
    public abstract class InspectableObject : Inspectable
    {
        public abstract string Name { get; }
        public abstract ObservableCollection<Inspectable> Inspectables { get; }
    }
}

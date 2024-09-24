using System.Collections.ObjectModel;
using System.Windows;

namespace CommonView.Controls
{
    public class Inspectable
    {
        public DataTemplate? HeaderTemplate { get; }
        public DataTemplate? ContentTemplate { get; }
    }


    public class InspectableObject
    {
        public static InspectableObject? Create(object o) { return null; }

        ObservableCollection<InspectableObject> Items { get; } = new ObservableCollection<InspectableObject>();
    }
}

using System.Collections.ObjectModel;
using Reactive.Bindings;

namespace OctbitEditor
{
    public class OutlinerItem
    {
        public OutlinerItem(string name = "New Item") { 
            Name.Value = name;
        }

        public ReactiveProperty<string> Name { get; } = new();
        public ReactiveProperty<bool> IsExpanded{ get; } = new(true);
        public ReactiveProperty<bool> IsSelected { get; } = new(false);
        public ObservableCollection<OutlinerItem> Children { get; } = new();
    }

    public class OutlinerVM
    {
        public OutlinerVM()
        {
            Children.Add(new OutlinerItem());
            Children.Add(new OutlinerItem());
            Children.Add(new OutlinerItem());

            Children[0].Children.Add(new OutlinerItem());
            Children[0].Children.Add(new OutlinerItem());
        }

        public ObservableCollection<OutlinerItem> Children { get; } = new();
    }
}

using CommonView.Controls;
using CommonView.History;
using OctbitEngine.Runtime;
using System.Collections.ObjectModel;

namespace OctbitEditor.Windows.Inspector
{
    public class InspectableRunttimeObject
    {
        public static ObservableCollection<Inspectable> Create(IRemoteObject obj)
        {
            var result = new ObservableCollection<Inspectable>();

            {
                foreach (var name in obj.TypeInfo.PropertyOrder)
                {
                    result.Add(new InspectableRuntimeProperty(obj,name));
                }
            }

            return result;
        }
    }

    public class InspectableRuntimeProperty : InspectableProperty
    {
        public override string DisplayName => PropertyInfo.Tags.TryGetValue("DisplayName", out var value) ? value : Name;

        public override string Name => PropertyInfo.Name;

        public override Type Type => PropertyInfo.TypeInfo.Type;

        public override object? Value
        {
            get => RemoteObject.GetValue(Name);
            set
            {
                if (value?.GetType() != Type)
                {
                    try
                    {
                        value = Convert.ChangeType(value, Type);
                    }
                    catch
                    {
                        return;
                    }
                }
                if (value == Value) return;

                var oldValue = Value;
                History.Record(
                    $"{Name}に値をセット : {value?.ToString()}",
                    () => { RemoteObject.SetValue(value,Name); RaisePropertyChanged(); },
                    () => { RemoteObject.SetValue(oldValue,Name); RaisePropertyChanged(); }
                );
            }
        }

        public override bool CanRead => PropertyInfo.CanRead;

        public override bool CanWrite => PropertyInfo.CanWrite;

        public override IReadOnlyDictionary<string, string> Tags => PropertyInfo.Tags;

        private IRemoteObject RemoteObject { get; }
        private IPropertyInfo PropertyInfo { get; }

        public InspectableRuntimeProperty(IRemoteObject obj, string name)
        {
            RemoteObject = obj;
            PropertyInfo = obj.TypeInfo.Properties[name];
        }
    }
}

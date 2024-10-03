using System.Collections.ObjectModel;
using System.Runtime.CompilerServices;
using System.Windows;

namespace CommonView.Controls
{
    public class Inspectable
    {
        public DataTemplate? HeaderTemplate { get; }
        public DataTemplate? ContentTemplate { get; }
    }



    public interface IObject
    {
        /// <summary>
        /// プロパティの値を取得します。
        /// </summary>
        T GetValue<T>([CallerMemberName] string? name = null) where T : notnull;

        /// <summary>
        /// プロパティの値を設定します。
        /// </summary>
        void SetValue<T>(T value, [CallerMemberName] string? name = null) where T : notnull;

    }

    public class InspectableObject
    {
        public static InspectableObject? Create(object o) { 
            return null; 
        }

        public string Name { get; } = string.Empty;

        public IObject Object { get; }

        private InspectableObject(IObject obj)
        {
            Object = obj;
        }


        T GetValue<T>([CallerMemberName] string? name = null) where T : notnull
        {
            return Object.GetValue<T>(name);
        }

        /// <summary>
        /// プロパティの値を設定します。
        /// </summary>
        void SetValue<T>(T value, [CallerMemberName] string? name = null) where T : notnull
        {
            var oldValue = GetValue<T>(name);
            if(value.Equals(oldValue)) return;

            History.History.Record(
                "値をセット",
                () => Object.SetValue(value, name),
                () => Object.SetValue(oldValue, name)
            );
        }
        ObservableCollection<InspectableObject> Items { get; } = new ObservableCollection<InspectableObject>();
    }
}

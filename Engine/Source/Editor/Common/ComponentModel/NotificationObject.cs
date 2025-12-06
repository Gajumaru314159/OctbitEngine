using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace Common.ComponentModel
{
    public class NotificationObject : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler? PropertyChanged;

        protected virtual void RaisePropertyChanged([CallerMemberName] string? propertyName = "")
        {
            var threadSafeHandler = Interlocked.CompareExchange(ref PropertyChanged, null, null);
            threadSafeHandler?.Invoke(this, new(propertyName));
        }

        protected bool RaisePropertyChangedIfSet<T>(ref T source, T value, string[]? relatedProperties = null, [CallerMemberName] string? propertyName = null)
        {
            if (EqualityComparer<T>.Default.Equals(source, value))
                return false;

            source = value;
            RaisePropertyChanged(propertyName);
            if (relatedProperties == null) return true;

            foreach (var p in relatedProperties)
                RaisePropertyChanged(p);

            return true;
        }

        protected bool RaisePropertyChangedIfSet<T>(ref T source, T value, string relatedProperty, [CallerMemberName] string? propertyName = null)
        {
            return RaisePropertyChangedIfSet(ref source, value, new[] { relatedProperty }, propertyName);
        }
    }

}

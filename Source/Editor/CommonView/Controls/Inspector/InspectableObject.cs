using Livet;
using System.Collections.ObjectModel;
using System.Runtime.CompilerServices;
using System.Windows;

namespace CommonView.Controls
{
    public class Inspectable : ViewModel
    {
    }

    public abstract class InspectableObject : Inspectable
    {
        public abstract string Name { get; }
        public abstract IList<Inspectable> Inspectables { get; }

        protected InspectableObject()
        {
        }

    }
}

using Livet;
using System.Collections.ObjectModel;
using System.Runtime.CompilerServices;
using System.Windows;

namespace CommonView.Controls
{
    public class Inspectable : ViewModel
    {
        public bool Visible
        #region
        {
            get => m_visible;
            set => RaisePropertyChangedIfSet(ref m_visible, value);
        }
        private bool m_visible = true;
        #endregion
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

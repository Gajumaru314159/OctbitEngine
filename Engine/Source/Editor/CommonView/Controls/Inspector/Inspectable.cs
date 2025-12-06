using Livet;

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
}

using CommonView;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;

namespace OctbitEditor
{
    /// <summary>
    /// Viewport.xaml の相互作用ロジック
    /// </summary>
    public partial class Viewport : UserControl
    {
        public Viewport()
        {
            InitializeComponent();
        }

        private void OnClicked(object sender, EventArgs e)
        {
            m_focusRoot.Focus();
        }

    }
}

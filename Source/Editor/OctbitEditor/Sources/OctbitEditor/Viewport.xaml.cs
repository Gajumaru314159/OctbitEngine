using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace OctbitEditor
{
    /// <summary>
    /// Viewport.xaml の相互作用ロジック
    /// </summary>
    public partial class Viewport : System.Windows.Controls.UserControl
    {
        [DllImport("user32")] private static extern int GetWindowLong(IntPtr hWnd, int nIndex);
        [DllImport("user32")] private static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);
        [DllImport("user32")] private static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);
        [DllImport("user32")] private static extern int MoveWindow(IntPtr hwnd, int x, int y, int nWidth, int nHeight, int bRepaint);

        private const int GWL_STYLE = -16;
        private const int GWL_EXSTYLE = -20;

        private const int WS_CAPTION = 0x00C00000;
        private const int WS_THICKFRAME = 0x00040000;
        private const int WS_BORDER = 0x00800000;
        private const int WS_SIZEBOX = 0x00040000;


        public Viewport()
        {
            InitializeComponent();

            if (Utils.IsInDesignMode==false)
            {
                Loaded += (sender, e) =>
                {
                    //return;
                    if (m_process == null)
                    {
                        var psi = new ProcessStartInfo()
                        {
                            FileName = @"D:\My\Productions\C++\OctbitEngine\Build\x64-Debug\Source\Application\Application.exe",
                            CreateNoWindow = true,
                            UseShellExecute = false,
                        };
                        m_process = Process.Start(psi);
                        if (m_process!=null)
                        {
                            IntPtr hWnd = IntPtr.Zero;
                            for (int i = 0; i<1000; ++i)
                            {
                                Thread.Sleep(100);
                                hWnd = m_process.MainWindowHandle;
                                if(hWnd!=IntPtr.Zero)
                                    break;
                            }

                            var oldStyle = GetWindowLong(hWnd, GWL_STYLE);
                            var oldExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
                            var style = oldStyle & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER & ~WS_SIZEBOX;
                            var exStyle = oldExStyle ;

                            SetParent(hWnd, m_panel.Handle);
                            SetWindowLong(hWnd, GWL_STYLE, style);
                            SetWindowLong(hWnd, GWL_EXSTYLE, exStyle);

                            SizeChanged += (sender, e) =>
                            {
                                MoveWindow(hWnd, 0, 0, (int)ActualWidth, (int)ActualHeight, 1);
                            };
                        }
                    }
                };
                Unloaded  += (sender, e) =>
                {
                    m_process?.Kill();
                    m_process =null;
                };
            }
        }

        private Process? m_process = null;

    }
}

using System;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using UserControl = System.Windows.Controls.UserControl;

namespace OctbitEditor
{
    /// <summary>
    /// Viewport.xaml の相互作用ロジック
    /// </summary>
    public partial class Viewport : UserControl
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

        private const int WS_POPUP = unchecked((int)0x80000000);
        private const int WS_VISIBLE = unchecked((int)0x10000000);


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
                        {
                            string taskkill = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.System), "taskkill.exe");
                            using (var procKiller = new System.Diagnostics.Process())
                            {
                                procKiller.StartInfo.FileName = taskkill;
                                procKiller.StartInfo.Arguments = "/F /IM OctbitApp.exe";
                                procKiller.StartInfo.CreateNoWindow = true;
                                procKiller.StartInfo.UseShellExecute = false;
                                procKiller.Start();
                                procKiller.WaitForExit();
                            }
                        }

                        var psi = new ProcessStartInfo()
                        {
                            FileName = @"D:\My\Productions\C++\OctbitEngine\Build\x64-Debug\Source\Application\OctbitApp.exe",
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

                            //var oldStyle = GetWindowLong(hWnd, GWL_STYLE);
                            //var oldExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
                            int style = WS_POPUP | WS_VISIBLE;
                            var exStyle = 0;// oldExStyle ;

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

                // TODO 強制終了時に子プロセスが残ってしまうので対処を考える
                Dispatcher.ShutdownStarted += (sender, e) =>
                {
                    m_process?.Kill();
                    m_process = null;
                };
            }
        }

        private Process? m_process = null;

    }
}

using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;

namespace CommonView.Controls
{
    /// <summary>
    /// InsertWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class InsertWindow : UserControl
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

        public static readonly DependencyProperty HwndProperty = DependencyProperty.Register(
                nameof(Hwnd),
                typeof(IntPtr),
                typeof(InsertWindow),
                new PropertyMetadata(IntPtr.Zero,new PropertyChangedCallback(OnHwndChanged)));

        public IntPtr Hwnd
        {
            get { return (IntPtr)GetValue(HwndProperty); }
            set { SetValue(HwndProperty, value); }
        }

        public InsertWindow()
        {
            InitializeComponent();
        }

        private static void OnHwndChanged(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            if (obj is not InsertWindow iw) return;
            if (e.NewValue is not IntPtr hWnd) return;

            int style = WS_POPUP | WS_VISIBLE;
            var exStyle = 0;// oldExStyle ;

            SetParent(hWnd, iw.m_panel.Handle);
            SetWindowLong(hWnd, GWL_STYLE, style);
            SetWindowLong(hWnd, GWL_EXSTYLE, exStyle);

            MoveWindow(hWnd, 0, 0, (int)iw.ActualWidth, (int)iw.ActualHeight, 1);
        }

        private void OnSizeChanged(object sender,SizeChangedEventArgs e)
        {
            if(Hwnd == IntPtr.Zero) return;
            if(ActualWidth<=0 || ActualHeight<=0) return;
            MoveWindow(Hwnd, 0, 0, (int)ActualWidth, (int)ActualHeight, 1);
        }

        private void OnLoaded(object sender, RoutedEventArgs e)
        {
            if (Window.GetWindow(this) is Window window)
            {
                var hsrc = HwndSource.FromVisual(this) as HwndSource;
                hsrc?.AddHook(WndProc);
            }
        }


        IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            switch (msg)
            {
                case 0x0231: // WM_ENTERSIZEMOVE
                    m_host.Visibility = Visibility.Collapsed;
                    break;
                case 0x0232: // WM_EXITSIZEMOVE
                    m_host.Visibility = Visibility.Visible;
                    break;
            }
            return IntPtr.Zero;
        }

        private void OnClick(object sender, EventArgs e)
        {
            if (sender is not InsertWindow iw) return;

            var f = iw.GetParentFast<FrameworkElement>();
            while (f!=null)
            {
                if (f.Name=="m_focusRoot")
                {
                    f.Focus();
                    break;
                }
                else
                {
                    f = f.GetParentFast<FrameworkElement>();
                }
            }
        }
    }
}

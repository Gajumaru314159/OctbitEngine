using System.IO;
using System.Windows;
using System.Windows.Controls;

namespace OctbitEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private static readonly string s_configName = "AvalonDock.config";

        public MainWindow()
        {
            InitializeComponent();
            // DataContext = new MainWindowVM();
        }

        private void OnLoaded(object? s,RoutedEventArgs e)
        {
            var serializer = new AvalonDock.Layout.Serialization.XmlLayoutSerializer(_dockingManager);
            serializer.LayoutSerializationCallback += (s, args) =>
            {
                args.Content =args.Content;
            };

            if (File.Exists(s_configName))
            {
                //serializer.Deserialize(s_configName);
            }
        }

        private void OnUnloaded(object? s,RoutedEventArgs e)
        {
            var serializer = new AvalonDock.Layout.Serialization.XmlLayoutSerializer(_dockingManager);
            serializer.Serialize(s_configName);
        }

        private void OnThemeColorChanged(object sender, SelectionChangedEventArgs e)
        {
            if (DataContext is not MainWindowVM vm) return;
            var theme = vm.ThemePreset;

            // AvalonDockのテーマ変更
            if (_dockingManager!=null)
            {
                _dockingManager.Theme = theme switch
                {
                    ThemePreset.Light => new AvalonDock.Themes.Vs2013LightTheme(),
                    ThemePreset.UnityLight => new AvalonDock.Themes.Vs2013LightTheme(),
                    _ => new AvalonDock.Themes.Vs2013DarkTheme()
                };
            }

            // ブラシ変更
            var resource = new ResourceDictionary() { Source = new Uri($"/CommonView;component/Themes/Colors/{theme}.xaml", UriKind.Relative) };
            Application.Current.Resources.MergedDictionaries[0] = resource;

            // Collection変更によるリロード
            var brushes = Application.Current.Resources.MergedDictionaries[2];
            Application.Current.Resources.MergedDictionaries.RemoveAt(2);
            Application.Current.Resources.MergedDictionaries.Insert(2,brushes);
        }


        private void MinimizeClick(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Minimized;
        }

        private void MaximizeClick(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Maximized;
        }

        private void RestoreDownClick(object sender, RoutedEventArgs e)
        {
            WindowState = WindowState.Normal;
        }

        private void CloseClick(object sender, RoutedEventArgs e)
        {
            Close();
        }

    }
}
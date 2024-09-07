using System.IO;
using System.Windows;
using System.Windows.Input;

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


    }
}
using System.Windows;

namespace OctbitEditor
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        protected override void OnStartup(StartupEventArgs e)
        {
            //var dialog = new StartupDialog();
            //dialog.ShowDialog();

            var window = new MainWindow();
            window.Show();
        }
    }

}

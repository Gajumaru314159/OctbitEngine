using OctbitEditor.Test;
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
            var w2 = new DockingTest();
            w2.Show();

            var window = new MainWindow();
            window.Show();

        }
    }

}

using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Windows;
using OctbitEditor.Dialog;
using Application = System.Windows.Application;

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

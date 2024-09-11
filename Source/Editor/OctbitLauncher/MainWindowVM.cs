using Common.Log;
using CommonView.Menu;
using Reactive.Bindings;
using System.Diagnostics;
using System.IO;
using System.Security.Policy;
using System.Windows.Input;
using System.Windows.Media.Imaging;

namespace OctbitLauncher
{
    public class CustomLinkVM
    {
        public CustomLinkVM(string name, string toolTip, string command,string icon)
        {
            Name=name;
            ToolTip=toolTip;
            Command=new DelegateCommand(() =>
            {
                try
                {
                    var pi = new ProcessStartInfo()
                    {
                        FileName = command,
                        UseShellExecute = true,
                    };
                    Process.Start(pi);
                }
                catch (Exception e)
                {
                    Log.Error(e.Message);
                }
            });
            try
            {
                Icon = new BitmapImage(new Uri(icon));
            }
            catch (Exception e)
            {
            }
        }


        public BitmapImage? Icon { get; } = null;
        public string Name { get; }
        public string ToolTip { get; }
        public ICommand Command { get; }
    }

    public class MainWindowVM
    {

        public MainWindowVM()
        {
            // TODO プロジェクトごとにカスタムリンクを設定できるようにする
            CustomLinks = new List<CustomLinkVM>
            {
                new("Github","OctbitEngineの使用マニュアルを開く","https://github.com/Gajumaru314159/OctbitEngine","file://D:\\My\\Productions\\C++\\OctbitEngine\\Source\\Editor\\OctbitLauncher\\Resources\\Icons\\document.png"),
                //new("Github","OctbitEngineの使用マニュアルを開く","https://github.com/Gajumaru314159/OctbitEngine","/OctbitLauncher;component/Resources/Icons/document.png"),
                new("ドキュメント","OctbitEngineの使用マニュアルを開く","","")
            };

            try
            {
                Icon.Value = new BitmapImage(new Uri("file://D:\\My\\Productions\\C++\\OctbitEngine\\Source\\Editor\\OctbitLauncher\\Resources\\Icons\\document.png"));
            }
            catch (Exception e)
            {
            }
        }

        public ReactivePropertySlim<BitmapImage?> Icon { get; } = new();
        public List<CustomLinkVM> CustomLinks { get; set; }
    }
}

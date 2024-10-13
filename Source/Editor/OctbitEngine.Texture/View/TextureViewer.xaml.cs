using OctbitEngine.Asset;
using OctbitEngine.Texture;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace OctbitEngine.View
{
    /// <summary>
    /// TextureViewer.xaml の相互作用ロジック
    /// </summary>
    [AssetEditor(typeof(OctbitEngine.Asset.Texture))]
    public partial class TextureViewer : Window
    {
        public TextureViewer(IAssetFile file)
        {
            DataContext = new TextureViewerVM(file);

            InitializeComponent();

            m_image.Source = new BitmapImage(new Uri(file.PhysicalPath));
        }
    }
}

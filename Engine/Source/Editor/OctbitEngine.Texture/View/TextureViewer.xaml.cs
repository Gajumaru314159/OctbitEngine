using OctbitEngine.Asset;
using OctbitEngine.Texture;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace OctbitEngine.Texture
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

            var image = new BitmapImage(new Uri(file.PhysicalPath));
            m_image.Source = image;
            m_canvas.Width = image.PixelWidth;
            m_canvas.Height = image.PixelHeight;
        }
    }
}

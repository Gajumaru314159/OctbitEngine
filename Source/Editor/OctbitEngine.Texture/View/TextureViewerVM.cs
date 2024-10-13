using CommonView.Controls;
using CommonView.Controls.Inspector;
using Livet;
using OctbitEngine.Asset;

namespace OctbitEngine.Texture
{
    public class TextureViewerVM :ViewModel
    {
        public TextureViewerVM(IAssetFile file)
        {
            m_file = file;

            Inspectables = InspectableReflectionObject.Create(file.Importer);
        }

        public IList<Inspectable> Inspectables { get; set; }

        private IAssetFile m_file;
    }
}

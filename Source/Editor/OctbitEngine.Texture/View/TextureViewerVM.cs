using CommonView.Controls;
using CommonView.Controls.Inspector;
using Livet;
using OctbitEngine.Asset;
using Reactive.Bindings;

namespace OctbitEngine.Texture
{
    public class TextureViewerVM :ViewModel
    {
        public TextureViewerVM(IAssetFile file)
        {
            m_file = file;

            Title.Value = file.Path;

            Inspectables = InspectableReflectionObject.Create(file.Importer);
        }

        public ReactivePropertySlim<string> Title { get; } = new();
        public IList<Inspectable> Inspectables { get; set; }

        private IAssetFile m_file;
    }
}

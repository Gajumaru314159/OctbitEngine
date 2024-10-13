using OctbitEngine.Config;

namespace OctbitEngine.Asset
{
    public class AssetConverter
    {

        public void Convert()
        {
        }

        private void ConvertImpl(IAsset asset)
        {
            var options = new IAsset.ConvertOptions()
            {
                Endian = Endian.Little,
                Platform = Platform.Windows,
            };

            var path = Path.Combine(WorkSpace.BinariesPath,Guid.NewGuid().ToString());
            

            using var fs = new FileStream(path, FileMode.Create);
            using var writer = new BinaryWriter(fs);

            asset.Convert(writer,options);

            // TODO 書き込みが正常に完了してから移動

        }

        private void CreateDatabase()
        {
        }
    }
}

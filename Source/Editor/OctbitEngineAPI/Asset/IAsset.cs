using System.IO;

namespace OctbitEngine.Asset
{
    public interface IAsset
    {
        public struct ConvertOptions
        {
            public Platform Platform;
            public Endian Endian;
        }

        string Name { get; }

        bool Convert(BinaryWriter writer,ConvertOptions options);
    }
}

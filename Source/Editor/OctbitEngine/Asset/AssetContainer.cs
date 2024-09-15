using System.Text;

namespace OctbitEngine.Asset
{
    public class AssetContainer : IAssetContainer
    {
        private Dictionary<string, ISerializable> _assets = new();
        private Dictionary<string, string> _map = new();

        public bool Contains(string name)
        {
            return _assets.ContainsKey(name);
        }
        public bool Add(string name, ISerializable asset)
        {
            if (Contains(name)) return false;
            _assets.Add(name, asset);
            return true;
        }
        public bool Map(string from, string to)
        {
            return _map.TryAdd(from, to);
        }

        public void Serialize(BinaryWriter writer)
        {
            // アセットコンテナのバイナリ形式は再考が必要
            // * アセット名が各データの先頭に書き込まれている
            // * アセットの参照方法が確定していない

            writer.Write((long)_assets.Count);

            Span<long> offsets = stackalloc long[_assets.Count];

            var start = writer.BaseStream.Position;

            // オフセット仮書き込み
            for (int i = 0; i<_assets.Count; ++i)
            {
                long offset = 0;
                writer.Write(offset);
            }

            // 実データ書き込み
            foreach (var (name, asset, i) in _assets.Select((kv, i) => (kv.Key, kv.Value, i)))
            {
                // オフセット取得
                offsets[i] = writer.BaseStream.Position;

                // 名前書き込み
                var u8name = UTF8Encoding.UTF8.GetBytes(name);
                writer.Write(u8name.Length);
                writer.Write(u8name);

                // データ書き込み
                // NOTE アライメントが必要？
                asset.Serialize(writer);
            }

            // オフセット書き込み
            {
                writer.BaseStream.Position = start;

                foreach (var offset in offsets)
                {
                    writer.Write(offset);
                }
            }

        }
    }
}

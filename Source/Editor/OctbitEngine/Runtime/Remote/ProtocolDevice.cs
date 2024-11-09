using Common.Hash;
using System.Text;

namespace OctbitEngine.Runtime
{
    internal class ProtocolDevice
    {
        private static readonly TimeSpan s_defaultTimeOut = TimeSpan.FromMilliseconds(-1);

        private record ResponseInfo(System.Reflection.MethodInfo OnReceived, System.Reflection.MethodInfo Deserialize);
        private Dictionary<uint, ResponseInfo> m_responces = new();

        private Dictionary<UInt64, Type> m_notices = new();
        private Dictionary<Type, UInt64> m_queries = new();
        private Dictionary<UInt64, Type> m_responses = new();

        private Dictionary<Type, MethodInfo> m_responseMethods = new();


        private Dictionary<UInt64, Action<object>> m_responseActions = new();


        UInt64 m_queryCount = 0;

        private NetworkDevice m_device;




        public bool IsConnected => m_device.IsConnected;


        record struct ProtocolHeader(Int64 size,UInt64 type);

        internal ProtocolDevice(IEnumerable<Type> types)
        {
            // クエリを収集
            m_notices = types.Where(t => t.IsAssignableTo(typeof(Notice))).ToDictionary(t => GetTypeHash(t), t => t);
            m_queries = types.Where(t => t.IsAssignableTo(typeof(Query))).ToDictionary(t => t, t => GetTypeHash(t));
            m_responses = types.Where(t => t.IsAssignableTo(typeof(Response))).ToDictionary(t => GetTypeHash(t), t => t);


            m_device = new NetworkDevice(System.Net.IPAddress.Loopback, 50000);
            m_device.DataReceived+=OnDataReceived;
        }

        public bool Send<T>(T query, Action<object>? response,bool isAsync, TimeSpan timeout) where T : Query
        {
            if (m_queries.TryGetValue(typeof(T), out var type))
            {
                var waitEvent = isAsync ? null : new AutoResetEvent(false);
                var queryId = Interlocked.Increment(ref m_queryCount);

                m_responseActions.Add(queryId, r =>
                {
                    response?.Invoke(r);
                    waitEvent?.Set();
                });

                // NOTE クエリ送信のたびにMemoryStreamを生成するのは非効率
                var stream = new MemoryStream();
                var writer = new BinaryWriter(stream);
                writer.Write((UInt64)0); // サイズを書き込むためのダミー
                writer.Write(type);
                writer.Write(queryId);
                query.Serialize(writer);
                writer.Seek(0, SeekOrigin.Begin);
                writer.Write((UInt64)stream.Length - 8 - 8 - 8);
                writer.Close();

                m_device.Send(stream.GetBuffer());

                return waitEvent?.WaitOne(timeout) ?? true;
            }

            throw new Exception($"{typeof(T).Name}はQueryとして登録されていません。定義しているアセンブリがQuery収集対象に含まれているか確認してください。");
        }

        private void OnDataReceived(byte[] buffer)
        {

            // TODO 分割送信対応
            var stream = new MemoryStream(buffer);
            var reader = new BinaryReader(stream);

            while (true)
            {
                var position = stream.Position;
                var remain = stream.Length - position;

                var header = new ProtocolHeader();
                header.size = 16 <= remain ? reader.ReadInt64() : 0;
                header.type = 16 <= remain ? reader.ReadUInt64() : 0;

                long headerSize = 8 + 8;
                var end = position + headerSize + header.size;

                // バッファサイズが足りなければシークを戻して終了
                if (remain < headerSize + header.size)
                {
                    stream.Seek(position, SeekOrigin.Begin);
                    break;
                }

                // Notice処理
                if (m_notices.TryGetValue(header.type,out var noticeType))
                {
                    var notice = (Notice)Activator.CreateInstance(noticeType)!;
                    notice.Deserialize(reader);
                    notice.OnReceive();
                }

                // Response処理
                if (m_responses.TryGetValue(header.type, out var responseType))
                {
                    var queryId = reader.ReadUInt64();
                    
                    if(m_responseActions.TryGetValue(queryId, out var action))
                    {
                        var response = (Response)Activator.CreateInstance(responseType)!;
                        response.Deserialize(reader);
                        action.Invoke(response);
                    }
                }

                stream.Seek(end, SeekOrigin.Begin);

            }


            /*
            var blockSize = reader.ReadUInt64();
            var protocolType = (ProtocolType)reader.ReadUInt64();
            var typeHash = reader.ReadUInt64();
            var handle = reader.ReadUInt64();



            var protpcolType = (ProtocolType)BitConverter.ToUInt32(buffer);
            var typeHash = BitConverter.ToUInt32(buffer.Slice(4));
            switch (protpcolType)
            {
                case ProtocolType.Notice:
                    if (m_notices.TryGetValue(typeHash, out var noticeType))
                    {
                        object? notice = null;
                        if(notice != null)
                        {
                            //s_noticeReceiberDictionary[typeHash].Invoke(null, new object[] { notice });
                        }
                    }
                    else
                    {
                        Log.Warning($"Unknown Protocol Type");
                    }
                    break;
                case ProtocolType.Query:
                    Log.Error($"ロジックエラー：Queryはエディタからの未送信可能です");
                    break;
                case ProtocolType.Responce:
                    var queryId = BitConverter.ToUInt32(buffer.Slice(8));
                    // レスポンス関数呼出し
                    break;
                default:
                    Log.Warning($"Unknown Protocol Type");
                    break;
            }
            */
        }


        private UInt64 GetTypeHash(Type type)
        {
            return Hash.FNV64(Encoding.UTF8.GetBytes(type.Name!));
        }

    }
}

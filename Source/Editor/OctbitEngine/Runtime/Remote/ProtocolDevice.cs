using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
using Common.Hash;
using Common.Log;

namespace OctbitEngine.Runtime
{

    internal class ProtocolDevice
    {
        private static readonly TimeSpan s_defaultTimeOut = TimeSpan.FromMilliseconds(-1);

        private record ResponseInfo(System.Reflection.MethodInfo OnReceived, System.Reflection.MethodInfo Deserialize);
        private Dictionary<UInt32, ResponseInfo> m_responces = new();

        private Dictionary<uint, Type> s_noticeTypeDictionary = new();
        private Dictionary<uint, Type> s_queryTypeDictionary = new();
        private Dictionary<uint, Type> s_responseTypeDictionary = new();
        private Dictionary<uint, MethodInfo> s_noticeReceiberDictionary = new();

        uint m_queryCount = 0;

        private NetworkDevice m_device;

        internal ProtocolDevice()
        {
            m_device = new NetworkDevice(System.Net.IPAddress.Loopback,50000);
            m_device.DataReceived+=OnDataReceived;

            // Responce型の派生形をリストアップ
            List<Type> derivedTypes = new List<Type>();

            Assembly assembly = typeof(Response).Assembly;
            Type[] types = assembly.GetTypes();

            foreach (Type type in types)
            {
                if (!type.IsSubclassOf(typeof(Response))) continue;
                
                var method = type.GetMethod("OnReceived", BindingFlags.Static | BindingFlags.Public);
                if (method is null) continue;

                method.Invoke(null, new object[] {});

            }
        }

        public bool Send<T>(T protocol, Action response,bool isAsync, int milliseconds = -1) where T : Query
        {
            var waittEvent = isAsync ? null : new AutoResetEvent(false);

            var typeHash = GetTypeHash(protocol.GetType());

            var queryId = Interlocked.Increment(ref m_queryCount);
            
            // quertId resultDelegate

            // データ送信

            // 待機と切断チェック

            return waittEvent?.WaitOne(milliseconds) ?? true;
        }

        private void OnDataReceived(ReadOnlySpan<byte> buffer)
        {
            var protpcolType = (ProtocolType)BitConverter.ToUInt32(buffer);
            var typeHash = BitConverter.ToUInt32(buffer.Slice(4));
            switch (protpcolType)
            {
                case ProtocolType.Notice:
                    if (s_noticeTypeDictionary.TryGetValue(typeHash, out var noticeType))
                    {
                        object? notice = null;
                        if(notice != null)
                        {
                            s_noticeReceiberDictionary[typeHash].Invoke(null, new object[] { notice });
                        }
                    }
                    else
                    {
                        Log.Warning($"Unknown Protocol Type");
                    }
                    break;
                case ProtocolType.Query:

                    break;
                case ProtocolType.Responce:
                    var queryId = BitConverter.ToUInt32(buffer.Slice(8));
                    // レスポンス関数呼出し
                    break;
                default:
                    break;
            }


            if (m_responces.TryGetValue(typeHash, out var response))
            {
                // var obj = response.Deserialize.Invoke(null,new object[] { (buffer.Slice(4) });
                // response.OnReceived.Invoke(null,new object[] { obj });
            }
            else
            {
                Log.Warning($"Unknown Protocol Type");
            }
        }

        private void RegisterProtocolType(Type type)
        {
            if (type.IsSubclassOf(typeof(Notice)))
            {
                s_noticeTypeDictionary.Add(GetTypeHash(type), type);
            }
            if (type.IsSubclassOf(typeof(Query)))
            {
                s_queryTypeDictionary.Add(GetTypeHash(type), type);
            }
            if (type.IsSubclassOf(typeof(Response)))
            {
                s_responseTypeDictionary.Add(GetTypeHash(type), type);
            }
        }


        private uint GetTypeHash(Type type)
        {
            return Hash.FNV32(Encoding.UTF8.GetBytes(type.FullName!));
        }

        enum ProtocolType
        {
            Notice = 0, // 一方的な通知
            Query,      // 返信が必要なリクエスト
            Responce,   // Querytに対する返信
        }

    }
}

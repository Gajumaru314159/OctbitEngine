using System.Diagnostics;
using System.Net;
using System.Net.Sockets;

namespace OctbitEngine.Runtime
{
    internal delegate void DataReceivedHandler(byte[] buffer);

    internal class NetworkDevice
    {
        private static readonly int s_bufferSize = 2 * 1024 * 1024;
        private Socket m_socket;
        private Socket? m_client = null;
        private byte[] m_buffer = new byte[s_bufferSize];

        internal event DataReceivedHandler? DataReceived;

        public bool IsConnected => m_client != null;

        internal NetworkDevice(IPAddress address,int port)
        {
            int maxConnections = 1;
            IPEndPoint endPoint = new IPEndPoint(address, port);
            m_socket = new Socket(address.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            m_socket.Bind(endPoint);
            m_socket.Listen(maxConnections);
            m_socket.NoDelay = true; // バッファリング無効化
            m_socket.BeginAccept(new AsyncCallback(OnAccept), m_socket);
        }

        private void OnAccept(IAsyncResult result)
        {
            if (m_client!=null) return;
            m_client = m_socket.EndAccept(result);
            HandleClientAsync(m_client);
        }

        private void HandleClientAsync(Socket client)
        {
            byte[] buffer = new byte[1024];
            int bytesRead;

            while ((bytesRead = client.Receive(buffer,SocketFlags.None,out SocketError errorCode)) != 0)
            {
                if (errorCode != SocketError.Success) break;
                // TODO バイト列で渡す
                DataReceived?.Invoke(buffer.AsSpan<byte>(0,bytesRead).ToArray());
            }

            client.Close();
        }

        internal void Send(ReadOnlySpan<byte> buffer)
        {
            Debug.Assert(m_client != null);
            m_client.Send(buffer);
        }
        private Dictionary<int, Type> m_queryTypes = new();
    }
}

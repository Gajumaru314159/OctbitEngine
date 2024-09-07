using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace OctbitEngine.Runtime
{
    internal delegate void DataReceivedHandler(ReadOnlySpan<byte> buffer);

    internal class NetworkDevice
    {
        private static readonly int s_bufferSize = 2 * 1024 * 1024;
        private Socket m_socket;
        private Socket? m_client = null;
        private byte[] m_buffer = new byte[s_bufferSize];

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

        internal event DataReceivedHandler? DataReceived;

        internal void WaitConnection()
        {
        }

        internal void Send(ReadOnlySpan<byte> buffer)
        {
            Debug.Assert(m_client != null);
            m_client.Send(buffer);
        }
        private void OnAccept(IAsyncResult result)
        {
            if (m_client!=null) return;
            // var  = m_socket.EndAccept(result);
            // 
            // _ = HandleClientAsync(m_client);
        }

        private static async Task HandleClientAsync(TcpClient client)
        {
            NetworkStream stream = client.GetStream();
            byte[] buffer = new byte[1024];
            int bytesRead;

            while ((bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length)) != 0)
            {
                string message = Encoding.UTF8.GetString(buffer, 0, bytesRead);
                Console.WriteLine("Received: " + message);

                // Echo the message back to the client
                byte[] response = Encoding.UTF8.GetBytes("Echo: " + message);
                await stream.WriteAsync(response, 0, response.Length);
            }

            client.Close();
        }

        private void OnReceive(ReadOnlySpan<byte> buffer)
        {
            Debug.Assert(m_client != null);

            //int queryType = 0;

            m_client.Receive(m_buffer);

            //if(m_queryTypes.TryGetValue(queryType,out Type type))
            //{
            //    // var o = Serializer.Deserialize(buffer,type);
            //}

            DataReceived?.Invoke(m_buffer);

        }

        private Dictionary<int, Type> m_queryTypes = new();
    }
}

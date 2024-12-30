using Common.Generic;
using Common.Hash;
using Common.Log;
using Common.Thread;
using Common.Math;
using OctbitEngine.Config;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Net;
using System.Reflection;
using System.Text;

namespace OctbitEngine.Runtime
{
    public sealed class Runtime : IRuntime
    {
        private int m_runtimeId = 0;
        private List<World> m_worlds = new();
        private Dictionary<int, RemoteObject> m_objects = new();
        private Process? m_process;
        private ProtocolDevice m_protocolDevice;
        private TypeInfoManager m_typeInfoManager;
        private RemoteObjectManager remoteObjectManager = new();

        public Runtime(IReadOnlySet<Type> types)
            : this(types,null)
        {
        }
        public Runtime(IReadOnlySet<Type> types,IProgress? progress)
        {
            progress?.SetRange(0, 100);
            progress?.SetMessage("ProtocolDeviceを生成");

            m_protocolDevice = new ProtocolDevice(types);

            progress?.SetRange(0, 100);
            progress?.SetMessage("Runtimeを起動中");

            startupRuntime();

            progress?.SetValue(10);
            progress?.SetMessage("Runtimeに接続中");

            ThreadUtility.WaitUntil(()=>m_protocolDevice.IsConnected, TimeSpan.FromSeconds(10), TimeSpan.FromMilliseconds(500));

            var output = "D:/reflection.json";
            Send(new GetReflectionQuery() { Output = output }, 
                (r) => {
                    if (r is not GetReflectionResponse response) return;
                    if (File.Exists(output))
                    {
                        var json = System.IO.File.ReadAllText(output);
                        File.Delete(output);
                        m_typeInfoManager = new TypeInfoManager(json, types);
                    }
                }
            );

            createViewportWorld();

        }

        private void OnDataReceived(ReadOnlySpan<byte> buffer)
        {

        }

        public IWorld ViewportWorld { get;private set; }

        public IWorld[] Worlds
        {
            get
            {
                lock (m_worlds)
                {
                    return m_worlds.ToArray();
                }
            }
        }

        public event Action<IRuntime>? Crashed;
        public event Action<IWorld>? WorldCreated;
        public event Action<IWorld>? WorldReleased;

        public ITypeInfo? FindTypeInfo(string name)
        {
            return m_typeInfoManager.Find(name);
        }

        public void Send<T>(T query) where T : Query
        {
            m_protocolDevice.Send(query, null, false, TimeSpan.FromHours(1));
        }

        public void Send<T>(T query, Action<object> responce, TimeSpan? timeout = null) where T : Query
        {
            m_protocolDevice.Send(query, responce, false, timeout?? TimeSpan.FromHours(1));
        }

        public void SendAsync<T>(T query, Action<object>? responce = null, TimeSpan? timeout = null) where T : Query
        {
            m_protocolDevice.Send(query, responce, true, timeout?? TimeSpan.FromHours(1));
        }

        public IRemoteObject CreateObject(TypeInfo type)
        {
            var id = Interlocked.Increment(ref m_runtimeId);
            var obj = new RemoteObject(this, type, id);
            lock (m_objects)
            {
                m_objects.Add(id, obj);
            }
            return obj;
        }

        public IRemoteObject? FindObject(int id)
        {
            lock (m_objects)
            {
                if (m_objects.TryGetValue(id, out var obj))
                {
                    return obj;
                }
            }
            return null;
        }

        private void startupRuntime()
        {
            // TODO JobObjectを使用してプロセスを自動終了できるようにする
            {
                string taskkill = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.System), "taskkill.exe");
                using (var procKiller = new System.Diagnostics.Process())
                {
                    procKiller.StartInfo.FileName = taskkill;
                    procKiller.StartInfo.Arguments = "/F /IM OctbitApp.exe";
                    procKiller.StartInfo.CreateNoWindow = true;
                    procKiller.StartInfo.UseShellExecute = false;
                    procKiller.Start();
                    procKiller.WaitForExit();
                }
            }

            
            var psi = new ProcessStartInfo()
            {
                FileName = Path.Combine(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location)!, @"../../x64-Debug/Source/Application/OctbitApp.exe"),
                WorkingDirectory = Path.Combine(WorkSpace.RootPath,"../../Assets"),
                UseShellExecute = false,
                CreateNoWindow = true,
                Arguments = "--editor",
            };

            if (File.Exists(psi.FileName))
            {
                try
                {
                    m_process = Process.Start(psi);
                }
                catch (Exception e)
                {
                    Log.Error($"Runtimeの起動に失敗\n{e.Message}");
                    return;
                }
            }

            if (m_process != null)
            {
                m_process.Exited += (sender, e) =>
                {
                    m_process = null;
                    Crashed?.Invoke(this);
                };
                m_process.EnableRaisingEvents = true;
            }
        }

        private void kill()
        {
            var process = m_process;
            if (process != null)
            {
                if (!process.HasExited)
                {
                    process.Kill();
                }
                process.Dispose();
            }
            m_process = null;
        }

        [MemberNotNull(nameof(ViewportWorld))]
        private void createViewportWorld()
        {
            lock (m_worlds)
            {
                var world = new World(this);
                ViewportWorld = world;
                m_worlds.Add(world);
            }
        }
    }
}

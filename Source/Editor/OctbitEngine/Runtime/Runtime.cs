using Common.Generic;
using Common.Log;
using Common.Thread;
using OctbitEngine.Config;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Net;

namespace OctbitEngine.Runtime
{
    public sealed class Runtime : Singleton<Runtime>, IRuntime
    {
        private int m_runtimeId = 0;
        private List<World> m_worlds = new();
        private Dictionary<int, RemoteObject> m_objects = new();
        private Process? m_process;
        private NetworkDevice m_network;
        private TypeInfoManager _typeInfoManager;

        public Runtime()
            : this(null)
        {
        }
        public Runtime(IProgress? progress)
        {
            m_network = new NetworkDevice(IPAddress.Loopback, 50000);

            progress?.SetRange(0, 100);
            progress?.SetMessage("Runtimeを起動中");

            startupRuntime();

            progress?.SetValue(10);
            progress?.SetMessage("Runtimeに接続中");

            connectRuntime();



            var infos = new TypeInfoArchive[]{
                new TypeInfoArchive
                {
                    Name = "ob::engine::World",
                },
                new TypeInfoArchive
                {
                    Name = "ob::engine::Scene",
                },
                new TypeInfoArchive
                {
                    Name = "ob::engine::Entity",
                },
                new TypeInfoArchive
                {
                    Name = "ob::engine::Component",
                }
            };

            var text = System.Text.Json.JsonSerializer.Serialize(infos);


            _typeInfoManager = new TypeInfoManager(text);

            createViewportWorld();

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
            return _typeInfoManager.Find(name);
        }

        public void Send<T>(T query) where T : Query
        {
            throw new NotImplementedException();
        }

        public void Send<T>(T query, Action<object> responce, TimeSpan? timeout = null) where T : Query
        {
            throw new NotImplementedException();
        }

        public void SendAsync<T>(T query, Action<object> responce, TimeSpan? timeout = null) where T : Query
        {
            throw new NotImplementedException();
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
                FileName = Path.Combine(WorkSpace.RootPath, @"Build\x64-Debug\Source\Application\OctbitApp.exe"),
                WorkingDirectory = WorkSpace.RootPath,
                UseShellExecute = false,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
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
                m_process.OutputDataReceived += (sender, e) =>
                {
                    Debug.WriteLine(e.Data);
                };
                m_process.ErrorDataReceived += (sender, e) =>
                {
                    Debug.WriteLine(e.Data);
                };
                m_process.BeginOutputReadLine();
                m_process.BeginErrorReadLine();
                m_process.Exited += (sender, e) =>
                {
                    m_process = null;
                    Crashed?.Invoke(this);
                };
                m_process.EnableRaisingEvents = true;

                try
                {
                    ThreadUtility.WaitUntil(() => m_process.MainWindowHandle != IntPtr.Zero, TimeSpan.FromSeconds(10), TimeSpan.FromMilliseconds(500));
                }
                catch (TimeoutException)
                {
                    Log.Error("Runtimeの起動に失敗しました");
                    kill();
                }
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

        private void connectRuntime()
        {

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

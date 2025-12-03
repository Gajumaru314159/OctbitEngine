using Common.Log;
using Common.Math;
using OctbitEngine.Asset;
using OctbitEngine.Runtime;
using System.Diagnostics;
using System.Reflection;

namespace OctbitEngine
{
    public class CoreSystem : ICoreSystem
    {
        public static CoreSystem? Instance;

        public IAssetManager AssetManager { get; }
        public IRuntime Runtime { get; }

        public CoreSystem()
        {
            Instance = this;
            Log.Logged += OnLogged;

            try
            {
                var pluginsFolder = Path.Combine(Directory.GetParent(Assembly.GetExecutingAssembly().Location)?.FullName??"", "..","Plugins");
                var searchPattern = "OctbitEngine.*.dll";
                if (Directory.Exists(pluginsFolder))
                {
                    string[] files = Directory.GetFiles(pluginsFolder, searchPattern, SearchOption.AllDirectories);

                    foreach (string file in files)
                    {
                        try
                        {
                            _pluginAssemblies.Add(Assembly.LoadFrom(file));
                        }
                        catch (Exception e)
                        {
                            Log.Error($"プラグインの読み込みに失敗\n{e.Message}");
                        }
                    }
                }
                else
                {
                    Log.Error($"プラグインフォルダが見つかりません ({pluginsFolder})");
                }
            }catch(Exception e)
            {
                Log.Error(e.Message);
            }

            m_types = _pluginAssemblies.Append(GetType().Assembly)
                .Append(GetType().Assembly) // OctbitEngine
                .Append(typeof(Query).Assembly) // OctbitEngineAPI
                .Append(typeof(Vector2).Assembly) // Common
                .SelectMany(a => a.GetTypes()).ToHashSet();

            AssetManager = new AssetManager(this);
            Runtime = new Runtime.Runtime(Types);
        }

        private void OnLogged(LogObject log)
        {
            string GetName(LogLevel level) => level switch
            {
                LogLevel.Error => "[Error]  ",
                LogLevel.Warn =>  "[Warning]",
                LogLevel.Info =>  "[Info]   ",
                LogLevel.Trace => "[Trace]  ",
                _ => ""
            };
            // TODO ロックオブジェクトを追加して書き込みの競合を防ぐ
            Debug.WriteLine($"{log.Timespamp} {GetName(log.Level)} : {log.Message}");
        }

        public IReadOnlyList<Assembly> PluginAssemblies => _pluginAssemblies;
        public IReadOnlySet<Type> Types => m_types;
        private List<Assembly> _pluginAssemblies = new();
        private HashSet<Type> m_types = new();
    }
}

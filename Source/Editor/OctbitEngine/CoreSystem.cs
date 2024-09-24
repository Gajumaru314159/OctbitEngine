using Common.Log;
using OctbitEngine.Runtime;
using OctbitEngine.Asset;
using OctbitEngine.Config;
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
                var pluginsFolder = Path.Combine(WorkSpace.RootPath, "Build","Plugins");
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

            AssetManager = new AssetManager();
            Runtime = new Runtime.Runtime();

        }

        private void OnLogged(LogObject log)
        {
            Debug.WriteLine($"{log.Timespamp} [{log.Level}] {log.Cateogory}\t: {log.Message}");
        }

        public IReadOnlyList<Assembly> PluginAssemblies => _pluginAssemblies;
        private List<Assembly> _pluginAssemblies = new();
    }
}

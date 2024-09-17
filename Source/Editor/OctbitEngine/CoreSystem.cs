using Common.Log;
using OctbitEngine.Asset;
using OctbitEngine.Config;
using System.Diagnostics;
using System.Reflection;

namespace OctbitEngine
{
    public class CoreSystem
    {
        public static CoreSystem? Instance;

        public CoreSystem()
        {
            Instance = this;
            Log.Logged += OnLogged;

            Log.Trace("CoreSystem initialized");
            Log.Info("CoreSystem initialized");
            Log.Warning("CoreSystem initialized");
            Log.Error("CoreSystem initialized");


            try
            {
                var pluginsFolder = Path.Combine(WorkSpace.RootPath, "Build","Plugins");
                var searchPattern = "OctbitEngine.*.dll";
                string[] files = Directory.GetFiles(pluginsFolder, searchPattern, SearchOption.AllDirectories);

                foreach (string file in files)
                {
                    try
                    {
                        _pluginAssemblies.Add(Assembly.LoadFrom(file));
                    } catch(Exception e)
                    {
                        Log.Error($"プラグインの読み込みに失敗\n{e.Message}");
                    }
                }
            }catch(Exception e)
            {
                Log.Error(e.Message);
            }
        }

        private void OnLogged(LogObject log)
        {
            Debug.WriteLine($"{log.Timespamp} [{log.Level}] {log.Cateogory}\t: {log.Message}");
        }

        public IReadOnlyList<Assembly> PluginAssemblies => _pluginAssemblies;
        private List<Assembly> _pluginAssemblies = new();
    }
}

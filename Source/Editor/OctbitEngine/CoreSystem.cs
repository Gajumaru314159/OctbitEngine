using Common.Log;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine
{
    internal class CoreSystem
    {
        internal CoreSystem()
        {
            Log.Logged += OnLogged;

            Log.Trace("CoreSystem initialized");
            Log.Info("CoreSystem initialized");
            Log.Warning("CoreSystem initialized");
            Log.Error("CoreSystem initialized");
        }

        private void OnLogged(LogObject log)
        {
            Debug.WriteLine($"{log.Timespamp} [{log.Level}] {log.Cateogory} : {log.Message}");
        }

    }
}

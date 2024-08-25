using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Common.Log
{
    public enum LogLevel
    {
        Trace,
        Info,
        Warning,
        Error
    }

    public record LogObject(
        DateTime Timespamp,
        LogLevel Level,
        string Message, 
        string Cateogory,
        string StackTrace
    );
    
    public delegate void LogEventHandler(LogObject log);

    public static class Log
    {
        public static void Trace(string message, string? category = null)
        {
            LogImpl(LogLevel.Trace, message, category);
        }
        public static void Info(string message,string? category = null)
        {
            LogImpl(LogLevel.Info, message, category);
        }
        public static void Warning(string message, string? category = null)
        {
            LogImpl(LogLevel.Warning, message, category);
        }
        public static void Error(string message, string? category = null)
        {
            LogImpl(LogLevel.Error, message, category);
        }

        private static void LogImpl(LogLevel level, string message, string? category = null)
        {
            var log = new LogObject(DateTime.Now, level, message, category ?? string.Empty,Environment.StackTrace);
            Logged?.Invoke(log);
        }

        public static LogEventHandler? Logged;
    }
}

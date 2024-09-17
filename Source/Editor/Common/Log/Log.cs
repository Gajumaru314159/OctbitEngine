using System.Diagnostics;

namespace Common.Log
{
    public enum LogLevel
    {
        Trace,
        Info,
        Warn,
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
        public static void BeginShink()
        {
            if (ShinkedObjects==null)
            {
                ShinkedObjects = new List<LogObject>();
            }
        }
        public static void EndShink()
        {
            if (ShinkedObjects!=null)
            {
                foreach(var item in ShinkedObjects)
                {
                    Logged?.Invoke(item);
                }
                ShinkedObjects = null;
            }
        }

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
            LogImpl(LogLevel.Warn, message, category);
        }
        public static void Error(string message, string? category = null)
        {
            LogImpl(LogLevel.Error, message, category);
        }

        private static void LogImpl(LogLevel level, string message, string? category = null)
        {
            var log = new LogObject(DateTime.Now, level, message, category ?? string.Empty,Environment.StackTrace);
            if (ShinkedObjects==null)
            {
                Logged?.Invoke(log);
            }
            else
            {
                ShinkedObjects.Add(log);
            }
        }

        public static LogEventHandler? Logged;
        public static List<LogObject>? ShinkedObjects = null; 
    }
}

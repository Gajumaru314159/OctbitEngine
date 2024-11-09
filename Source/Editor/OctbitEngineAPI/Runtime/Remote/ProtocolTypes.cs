using Common.Log;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.Runtime
{
    public interface Notice
    {
        void Deserialize(BinaryReader reader);
        void OnReceive();
    }
    public interface Query
    {
        void Serialize(BinaryWriter writer);
    }

    public interface Response
    {
        void Deserialize(BinaryReader reader);
    }



    public class LogNotice : Notice
    {
        public void Deserialize(BinaryReader reader)
        {
            LogLevel = reader.ReadUInt32() switch
            {
                // 0 => LogLevel.Fatal,
                1 => LogLevel.Error,
                2 => LogLevel.Warn,
                3 => LogLevel.Info,
                _ => LogLevel.Trace
            };

            var length = reader.ReadUInt64();
            var bytes = reader.ReadBytes((int)length);
            Message = Encoding.UTF8.GetString(bytes);
        }
        public void OnReceive()
        {
            switch (LogLevel)
            {
                case LogLevel.Error:
                    Log.Error(Message);
                    break;
                case LogLevel.Warn:
                    Log.Warning(Message);
                    break;
                case LogLevel.Info:
                    Log.Info(Message);
                    break;
                case LogLevel.Trace:
                    Log.Trace(Message);
                    break;
            }
        }
        public string Message { get; private set; } = string.Empty;
        public LogLevel LogLevel { get; private set; } = LogLevel.Info;
    }

    public class AddViewportQuery : Query
    {
        public void Serialize(BinaryWriter writer)
        {

        }
    }
    public class AddViewportResponse : Response
    {
        public IntPtr WindowHandle;

        public void Deserialize(BinaryReader reader)
        {
            if (IntPtr.Size == 8)
            {
                WindowHandle = (IntPtr)reader.ReadUInt64();
            }
            if (IntPtr.Size == 4)
            {
                WindowHandle = (IntPtr)reader.ReadUInt32();
            }
        }
    }

}

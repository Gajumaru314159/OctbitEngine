using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http.Headers;
using System.Text;
using System.Threading.Tasks;

namespace Common.Thread
{
    public static class ThreadUtility
    {

        public static void WaitUntil(Func<bool> pred, TimeSpan timeout, TimeSpan checkFrequency)
        {
            if (checkFrequency <= TimeSpan.Zero)
            {
                checkFrequency = TimeSpan.FromMilliseconds(100);
            }
            while (!pred())
            {
                if (timeout <= TimeSpan.Zero)
                {
                    throw new TimeoutException("Timeout waiting for condition to be true.");
                }
                System.Threading.Thread.Sleep(checkFrequency);
                timeout -= checkFrequency;
            }
        }
        public static T WaitAndGet<T>(Func<T?> getter, TimeSpan timeout, TimeSpan checkFrequency) where T : class
        {
            if (checkFrequency <= TimeSpan.Zero)
            {
                checkFrequency = TimeSpan.FromMilliseconds(100);
            }

            T? retval = getter();
            while (retval == null)
            {
                if (timeout <= TimeSpan.Zero)
                {
                    throw new TimeoutException("Timeout waiting for condition to be true.");
                }
                System.Threading.Thread.Sleep(checkFrequency);
                timeout -= checkFrequency;
                retval = getter();
            }
            return retval;
        }

        public static void ExecuteInBackground(Action action,Action<Exception>? exceptionHandler = null)
        {
            if(System.Threading.Thread.CurrentThread == MainThread)
            {
                Task.Run(() => execute(action, exceptionHandler));
            }
            else
            {
                execute(action,exceptionHandler);
            }
        }

        // MainThreadの概念はView層のものなので、ここで使うべきではない？
        /*
        public static void ExecuteInMainThread(Action action, Action<Exception>? exceptionHandler = null)
        {
            if (System.Threading.Thread.CurrentThread == MainThread)
            {
                execute(action, exceptionHandler);
            }
            else
            {
                MainDispatcher?.Invoke(()=>execute(action,exceptionHandler));
            }
        }

        public static T? GetFromMainThread<T>(Func<T> getter)
        {
            if(CurrentThread == MainThread)
            {
                return getter();
            }
            else
            {
                return MainDispatcher?.Invoke(()=>getter.Invoke())??default;
            }
        }
        */

        private static void execute(Action action, Action<Exception>? exceptionHandler)
        {
            try
            {
                action.Invoke();
            } catch (Exception e)
            {
                if (exceptionHandler!=null)
                    exceptionHandler?.Invoke(e);
                else
                    throw;
            }
        }
        public static System.Threading.Thread MainThread { get; } = System.Threading.Thread.CurrentThread;
        public static System.Threading.Thread CurrentThread { get; } = System.Threading.Thread.CurrentThread;
    }
}

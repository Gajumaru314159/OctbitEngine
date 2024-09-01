using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Common.Generic
{
    public static class Utility
    {
        public static T LockGet<T>(this object @lock, Func<T> func)
        {
            lock (@lock)
                return func();
        }
        public static void LockDo(this object @lock, Action func)
        {
            lock (@lock)
                func();
        }

    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Common.Linq
{
    public static class LinqUtility
    {        public static T? FirstOrNull<T>(this IEnumerable<T> items) where T : class
        {
            return items.DefaultIfEmpty(null).FirstOrDefault();
        }
    }
}

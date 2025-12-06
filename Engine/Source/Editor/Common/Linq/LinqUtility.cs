using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Common.Linq
{
    public static class LinqUtility
    {
        public static T? FirstOrNull<T>(this IEnumerable<T> items) where T : class
        {
            return items.DefaultIfEmpty(null).FirstOrDefault();
        }
        public static T? FirstOrNull<T>(this IEnumerable<T> items, Func<T,bool> predicate) where T : class
        {
            return items.DefaultIfEmpty(null).FirstOrDefault(i=>predicate(i!));
        }

        public static IEnumerable<T> NotNull<T>(this IEnumerable<T?>? item)
        {
            return item?.Where(x => x != null).Select(x => x!)??Array.Empty<T>();
        }

        public static void ForEach<T>(this IEnumerable<T> items, Action<T> action)
        {
            foreach (var item in items)
            {
                action(item);
            }
        }
        public static void ForEach<T>(this IEnumerable<T> items, Action<T,int> action)
        {
            int index = 0;
            foreach (var item in items)
            {
                action(item,index);
                index++;
            }
        }
    }
}

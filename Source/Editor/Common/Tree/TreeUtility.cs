using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Common.Tree
{
    public static class TreeUtility
    {
        public static IEnumerable<T> Ancestor<T>(this T? node, Func<T, T?> parent)
        {
            while (node != null)
            {
                yield return node;
                node = parent(node!);
            }
        }
    }
}

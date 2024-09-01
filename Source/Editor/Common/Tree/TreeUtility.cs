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
        public static bool AnyAncestor<T>(this T? node, Func<T, T?> parent, Func<T, bool> pred)
        {
            return node.Ancestor(parent).Any(pred);
        }
        public static bool AllAncestor<T>(this T? node, Func<T, T?> parent, Func<T, bool> pred)
        {
            return node.Ancestor(parent).All(pred);
        }

        public static bool IsAncestorOf<T>(this T? node, Func<T, T?> parent)
        {
            return node.AnyAncestor(parent, n => n.Equals(node));
        }
    }
}

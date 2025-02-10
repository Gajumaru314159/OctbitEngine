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
                yield return node!;
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
            return node.AnyAncestor(parent, n => n!.Equals(node));
        }

        public static IEnumerable<T> DepthFirst<T>(this T? node, Func<T, IEnumerable<T>> children)
        {
            if (node != null)
            {
                yield return node;

                foreach (var child in children(node))
                {
                    foreach (var descendant in child.DepthFirst(children))
                    {
                        yield return descendant;
                    }
                }
            }
        }

        public static ISet<T> SelectRoot<T>(this IEnumerable<T> nodes, Func<T, T?> parent)
        {
            var result = new HashSet<T>();
            var set = nodes.ToHashSet();

            void select(T node)
            {
                if (parent(node)?.AllAncestor(parent, i => !set.Contains(i))??true)
                {
                    result.Add(node);
                }
            }

            foreach (var node in nodes)
            {
                select(node);
            }

            return result;
        }
    }
}

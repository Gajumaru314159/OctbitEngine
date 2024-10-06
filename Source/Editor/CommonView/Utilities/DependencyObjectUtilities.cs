using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;

namespace CommonView
{
    public static class DependencyObjectUtilities
    {
        public static T? GetParentFast<T>(this DependencyObject self) where T : class?
        {
            var parent = self;
            while (true)
            {
                if (parent is not Visual || parent is not Visual3D)
                    return default;

                parent = VisualTreeHelper.GetParent(parent);
                switch (parent)
                {
                    case T t:
                        return t;
                    case null:
                        return default;
                }
            }
        }
    }
}

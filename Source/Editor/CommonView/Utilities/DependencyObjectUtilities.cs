using System.Windows;
using System.Windows.Media;

namespace CommonView
{
    public static class DependencyObjectUtilities
    {
        public static T? GetParentFast<T>(this DependencyObject self) where T : class?
        {
            var parent = self;
            try
            {
                while (true)
                {
                    parent = VisualTreeHelper.GetParent(parent);
                    switch (parent)
                    {
                        case T t:
                            return t;
                        case null:
                            return default;
                    }
                }
            }catch
            {
                return default;
            }
        }
    }
}

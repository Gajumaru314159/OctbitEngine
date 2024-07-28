using System.ComponentModel;
using System.Windows;

namespace OctbitEditor
{
    public static class Utils
    {       
        public static bool IsInDesignMode
            => (bool)(DesignerProperties.IsInDesignModeProperty.GetMetadata(typeof(DependencyObject)).DefaultValue);

    }
}

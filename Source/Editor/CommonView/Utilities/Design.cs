using System.ComponentModel;
using System.Windows;

namespace CommonView
{
    public static class Design
    {       
        public static bool IsInDesignMode
            => (bool)(DesignerProperties.IsInDesignModeProperty.GetMetadata(typeof(DependencyObject)).DefaultValue);

    }
}

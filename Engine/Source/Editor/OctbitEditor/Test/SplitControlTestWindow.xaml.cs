using CommonView.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace OctbitEditor.Test
{
    /// <summary>
    /// Interaction logic for SplitControlTestWindow.xaml
    /// </summary>
    public partial class SplitControlTestWindow : Window
    {
        public SplitControlTestWindow()
        {
            InitializeComponent();
        }
    }

    public class SplitControlTestWindowSelector : DataTemplateSelector
    {
        static SplitControlTestWindowSelector()
        {

        }

        public override DataTemplate? SelectTemplate(object item, DependencyObject container)
        {
            if (container is not FrameworkElement f) return null;
            if (item is SplitControlTestWindowVMSample p)
            {
                return f.FindResource("SplitControlTestWindowVMSampleTemplate") as DataTemplate;
            }
            // TODO List
            // TODO Dictionary
            return null;
        }

    }
}

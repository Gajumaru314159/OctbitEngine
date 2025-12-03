using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEditor.Test
{
    public class SplitControlTestWindowVMSample
    {
        public int Value { get; set; }
    }
    public class SplitControlTestWindowVMSample2
    {
        public int Value { get; set; }
    }
    public class SplitControlTestWindowVM
    {
        public SplitControlTestWindowVM()
        {
            ItemsSource.Add(new SplitControlTestWindowVMSample { Value = 1 });
            ItemsSource.Add(new SplitControlTestWindowVMSample2 { Value = 2 });
            ItemsSource.Add(new SplitControlTestWindowVMSample { Value = 3 });
        }


        public ObservableCollection<object> ItemsSource { get; } = new();
    }
}

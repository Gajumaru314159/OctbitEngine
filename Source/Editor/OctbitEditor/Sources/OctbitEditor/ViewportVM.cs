using Reactive.Bindings;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEditor
{
    public class ViewportVM
    {
        public ViewportVM()
        {

        }


        public ReactivePropertySlim<bool> ToolIsEnabled { get; } = new (true);
        public ReactivePropertySlim<bool> CoordinateIsEnabled { get; } = new(true);
        public ReactivePropertySlim<bool> SnapIsEnabled { get; } = new(true);
        public ReactivePropertySlim<bool> ShadingIsEnabled { get; } = new(true);
        public ReactivePropertySlim<bool> LayerIsEnabled { get; } = new(true);
    }
}

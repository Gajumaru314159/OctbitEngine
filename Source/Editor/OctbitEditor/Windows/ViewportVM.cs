using OctbitEditor.Windows;
using Reactive.Bindings;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEditor
{
    public enum OperatorMode
    {
        Select,
        Move,
        Rotate,
        Scale,
    }
    public enum ShadingMode
    {
        Shading,
        Wireframe,
        ShadingWireframe,
    }

    public class ViewportVM : TabBase
    {
        public ViewportVM()
            : base("Viewport")
        {
            PlayCommand.Subscribe(_ => IsPlaying.Value = !IsPlaying.Value);
            SelectCommand.Subscribe(_ => OpMode.Value = OperatorMode.Select);
            MoveCommand.Subscribe(_ => OpMode.Value = OperatorMode.Move);
            RotateCommand.Subscribe(_ => OpMode.Value = OperatorMode.Rotate);
            ScaleCommand.Subscribe(_ => OpMode.Value = OperatorMode.Scale);
            ToggleCoordinateCommand.Subscribe(_ => IsWorldCoordinate.Value = !IsWorldCoordinate.Value);
            ToggleSnapMoveCommand.Subscribe(_ => SnapMoveIsEnabled.Value = !SnapMoveIsEnabled.Value);
            ToggleSnapRotateCommand.Subscribe(_ => SnapRotateIsEnabled.Value = !SnapRotateIsEnabled.Value);
            ToggleSnapScaleCommand.Subscribe(_ => SnapScaleIsEnabled.Value = !SnapScaleIsEnabled.Value);
        }


        public ReactivePropertySlim<bool> PlayIsEnabled { get; } = new (true);
        public ReactivePropertySlim<bool> ToolIsEnabled { get; } = new (true);
        public ReactivePropertySlim<bool> CoordinateIsEnabled { get; } = new(true);
        public ReactivePropertySlim<bool> SnapIsEnabled { get; } = new(true);
        public ReactivePropertySlim<bool> ShadingIsEnabled { get; } = new(true);
        public ReactivePropertySlim<bool> LayerIsEnabled { get; } = new(true);


        public ReactivePropertySlim<bool> IsPlaying { get; } = new(false);

        public ReactivePropertySlim<OperatorMode> OpMode { get; } = new(OctbitEditor.OperatorMode.Select);
        public ReactivePropertySlim<bool> IsWorldCoordinate{ get; } = new(false);
        public ReactivePropertySlim<ShadingMode> ShadingMode_ { get; } = new(ShadingMode.Shading);


        public ReactivePropertySlim<bool> SnapMoveIsEnabled { get; } = new(false);
        public ReactivePropertySlim<bool> SnapRotateIsEnabled { get; } = new(false);
        public ReactivePropertySlim<bool> SnapScaleIsEnabled { get; } = new(false);

        public ReactiveCommandSlim PlayCommand { get; } = new();
        public ReactiveCommandSlim SelectCommand { get; } = new();
        public ReactiveCommandSlim MoveCommand { get; } = new();
        public ReactiveCommandSlim RotateCommand { get; } = new();
        public ReactiveCommandSlim ScaleCommand { get; } = new();
        public ReactiveCommandSlim ToggleCoordinateCommand { get; } = new();
        public ReactiveCommandSlim ToggleSnapMoveCommand { get; } = new();
        public ReactiveCommandSlim ToggleSnapRotateCommand { get; } = new();
        public ReactiveCommandSlim ToggleSnapScaleCommand { get; } = new();
    }
}

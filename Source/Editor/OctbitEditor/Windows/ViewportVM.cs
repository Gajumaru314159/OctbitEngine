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

    public class ViewportVM
    {
        public ViewportVM()
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

        public ReactiveCommand PlayCommand { get; } = new();
        public ReactiveCommand SelectCommand { get; } = new();
        public ReactiveCommand MoveCommand { get; } = new();
        public ReactiveCommand RotateCommand { get; } = new();
        public ReactiveCommand ScaleCommand { get; } = new();
        public ReactiveCommand ToggleCoordinateCommand { get; } = new();
        public ReactiveCommand ToggleSnapMoveCommand { get; } = new();
        public ReactiveCommand ToggleSnapRotateCommand { get; } = new();
        public ReactiveCommand ToggleSnapScaleCommand { get; } = new();
    }
}

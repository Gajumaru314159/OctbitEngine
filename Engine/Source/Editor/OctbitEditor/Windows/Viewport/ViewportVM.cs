using Common.Hash;
using CommonView.Menu;
using OctbitEngine.Runtime;
using Reactive.Bindings;
using System.IO;
using System.Text;
using System.Windows.Controls;
using System.Windows.Input;

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

    public class ViewportVM : DocumentBase
    {
        private IRuntime Runtime { get; }

        public ViewportVM(IRuntime runtime)
            : base("Viewport")
        {
            Runtime = runtime;

            PlayCommand.Subscribe(_ => IsPlaying.Value = !IsPlaying.Value);
            SelectCommand.Subscribe(_ => OpMode.Value = OperatorMode.Select);
            MoveCommand.Subscribe(_ => OpMode.Value = OperatorMode.Move);
            RotateCommand.Subscribe(_ => OpMode.Value = OperatorMode.Rotate);
            ScaleCommand.Subscribe(_ => OpMode.Value = OperatorMode.Scale);
            ToggleCoordinateCommand.Subscribe(_ => IsWorldCoordinate.Value = !IsWorldCoordinate.Value);
            ToggleSnapMoveCommand.Subscribe(_ => SnapMoveIsEnabled.Value = !SnapMoveIsEnabled.Value);
            ToggleSnapRotateCommand.Subscribe(_ => SnapRotateIsEnabled.Value = !SnapRotateIsEnabled.Value);
            ToggleSnapScaleCommand.Subscribe(_ => SnapScaleIsEnabled.Value = !SnapScaleIsEnabled.Value);

            ToolBarDock.Subscribe(_ => RaisePropertyChanged(nameof(ToolBarOrientation)));



            var query = new AddViewportQuery();
            Runtime.Send(query, r =>
            {
                if (r is not AddViewportResponse responce) return;
                Hwnd.Value = responce.WindowHandle;
            });
        }

        public ReactiveProperty<IntPtr> Hwnd { get; } = new(IntPtr.Zero);


        public ReactivePropertySlim<Dock> ToolBarDock { get; } = new(Dock.Top);
        public IEnumerable<Dock> ToolBarDocks => Enum.GetValues<Dock>();
        public Orientation ToolBarOrientation
            => ToolBarDock.Value switch
            {
                Dock.Top => Orientation.Horizontal,
                Dock.Bottom => Orientation.Horizontal,
                Dock.Left => Orientation.Vertical,
                Dock.Right => Orientation.Vertical,
                _ => Orientation.Horizontal
            };

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

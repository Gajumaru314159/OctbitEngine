using System.Windows;
using System.Windows.Documents;
using System.Windows.Media;

namespace CommonView.Controls
{
    public class DockingAdorner : Adorner
    {
        private readonly Rect _dropArea;
        private readonly Brush _brush;

        public enum PositionType
        {
            Top,
            Bottom,
            Left,
            Right,
            Center
        };

        public PositionType Position { get; set; }

        public DockingAdorner(UIElement adornedElement, Rect dropArea) : base(adornedElement)
        {
            _dropArea = dropArea;
            _brush = new SolidColorBrush(Color.FromArgb(128, 255, 0, 0)); // Semi-transparent red
        }

        protected override void OnRender(DrawingContext drawingContext)
        {
            var geometry = new StreamGeometry();
            using (var context = geometry.Open())
            {
                // Define trapezoidal shape
                context.BeginFigure(new Point(_dropArea.Left, _dropArea.Top), true, true);
                context.LineTo(new Point(_dropArea.Right, _dropArea.Top),false,false);
                context.LineTo(new Point(_dropArea.Right - 20, _dropArea.Bottom), false, false);
                context.LineTo(new Point(_dropArea.Left + 20, _dropArea.Bottom), false, false);
                context.Close();
            }

            drawingContext.DrawGeometry(_brush, null, geometry);
        }
    }
}
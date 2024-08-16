using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Threading;

namespace CommonView
{
    public class StyledSolidColorBrush : Brush
    {
        public static readonly DependencyProperty ColorProperty = DependencyProperty.Register(
            "Color",
            typeof(Color),
            typeof(StyledSolidColorBrush),
            new FrameworkPropertyMetadata(Colors.Transparent, FrameworkPropertyMetadataOptions.AffectsRender));

        public static readonly DependencyProperty AffectColorProperty = DependencyProperty.Register(
            "AffectColor",
            typeof(Color),
            typeof(StyledSolidColorBrush),
            new FrameworkPropertyMetadata(Colors.Transparent, FrameworkPropertyMetadataOptions.AffectsRender));

        public StyledSolidColorBrush()
        {
        }


        public Color Color
        {
            get => (Color)GetValue(ColorProperty);
            set => SetValue(ColorProperty, value);
        }
        public Color AffectColor
        {
            get => (Color)GetValue(AffectColorProperty);
            set => SetValue(AffectColorProperty, value);
        }

        protected override Freezable CreateInstanceCore()
        {
            return new StyledSolidColorBrush();
        }
    }
}

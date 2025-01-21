using AvalonDock.Properties;
using Common.Math;
using Livet;
using System.Reactive.Linq;
using System.Windows;

namespace CommonView.Controls
{
    public sealed partial class Vector3ResourceDictionary
    {
        public static Vector3ResourceDictionary Instance { get; } = new();
        static Vector3ResourceDictionary()
        {
            var resource = new Vector3ResourceDictionary();
            resource.InitializeComponent();

            EditorRegistory.Register(typeof(EnumEditor), (DataTemplate)resource["EnumEditor"]);
            EditorRegistory.Register(typeof(BoolEditor), (DataTemplate)resource["BoolEditor"]);
            EditorRegistory.Register(typeof(ScalarEditor), (DataTemplate)resource["ScalarEditor"]);
            EditorRegistory.Register(typeof(StringEditor), (DataTemplate)resource["StringEditor"]);
            EditorRegistory.Register(typeof(Vector2Editor), (DataTemplate)resource["Vector2Editor"]);
            EditorRegistory.Register(typeof(Vector3Editor), (DataTemplate)resource["Vector3Editor"]);
        }
    }


}

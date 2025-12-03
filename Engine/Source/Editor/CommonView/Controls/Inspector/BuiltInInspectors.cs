using Common.Linq;
using Common.Math;
using CommonView.History;
using CommonView.Menu;
using Livet;
using Reactive.Bindings.Extensions;
using System;
using System.Reactive.Linq;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace CommonView.Controls
{
    [AttributeUsage(AttributeTargets.Class|AttributeTargets.Struct|AttributeTargets.Enum)]
    public sealed class PropertyEditorAttribute : Attribute
    {
        public Type[] Targets { get; }
        public PropertyEditorAttribute(params Type[] targets)
        {
            Targets = targets;
        }

    }

    [AttributeUsage(AttributeTargets.Class|AttributeTargets.Struct)]
    public sealed class ObjectEditorAttribute : Attribute
    {
        public Type[] Targets { get; }
        public ObjectEditorAttribute(params Type[] targets)
        {
            Targets = targets;
        }

    }

    public class EditorSelector : DataTemplateSelector
    {
        public override DataTemplate? SelectTemplate(object vm, DependencyObject container)
        {
            if (container is not FrameworkElement f) return null;
            if (vm is null) return null;
            return
                EditorRegistory.FindTemplate(vm.GetType())
                ?? f.FindResource("UnknownEditor") as DataTemplate;
        }
    }


    public static class EditorRegistory
    {

        private static Dictionary<Type, DataTemplate> _templates = new();
        private static Dictionary<Type, Type> _viewModels= new();

        static EditorRegistory()
        {
            AddAssembly(typeof(EditorRegistory).Assembly);
            
            // TODO リソース登録
            int a = Vector3ResourceDictionary.Instance.Count;
        }

        public static void AddAssembly(Assembly assembly)
        {
            var types = assembly.GetTypes();
            foreach (var type in types)
            {
                if (type.IsSubclassOf(typeof(Editor)))
                {
                    var targets = type.GetCustomAttribute<PropertyEditorAttribute>();
                    if (targets is not null)
                    {
                        foreach (var target in targets.Targets)
                        {
                            _viewModels.TryAdd(target, type);
                        }
                    }
                }
            }
        }

        public static bool Register(Type type, DataTemplate? template)
        {
            if (template is null) return false;
            return _templates.TryAdd(type, template);
        }

        internal static DataTemplate? FindTemplate(Type type)
        {
            _templates.TryGetValue(type, out DataTemplate? template);
            return template;
        }
        public static Type? FindViewModel(Type? type)
        {
            if(type is null) return null;
            _viewModels.TryGetValue(type, out Type? vm);
            return vm;
        }

    }

    public class Editor : Inspectable
    {
        public static string MultiValuesText = "";
        protected Editor(InspectableProperty[] targets,params string[] syncs)
        {
            Targets = targets;
            Observable
                .Merge(Targets.Select(i=>i.ObserveProperty(i=>i.Value)))
                .Subscribe(_=> { RaisePropertyChanged(nameof(IsSameValues)); syncs.Append("Value").Append("IsSameValues").ForEach(RaisePropertyChanged); });
        }
        public InspectableProperty[] Targets { get; }
        public InspectableProperty Target => Targets[0];
        public bool IsSameValues => Targets.All(i => Equals(i.Value, Target.Value));

        public Type Type => Target.Type;
        public string Name => Target.DisplayName;
        public string? ToolTip => Target.Tags.TryGetValue("ToolTip", out string? value)?value:null;
        public bool CanRead => Target.CanRead;
        public bool CanWrite => Target.CanWrite;
    }

    public class EnumEditor : Editor
    {
        public EnumEditor(InspectableProperty[] targets) : base(targets)
        {
            Items = Type.IsEnum ? Enum.GetNames(Type) : Array.Empty<string>();
        }

        public string Value
        {
            // NOTE ComboBoxにはItems以外の要素が表示できないためMultiValuesTextを表示できていない
            get => IsSameValues ? Target.Value!.ToString()??string.Empty : MultiValuesText;
            set
            {
                if (Enum.TryParse(Type, value, out var result))
                {
                    var oldValues = Targets.Select(i => i.Value).ToArray();
                    History.History.Record(
                        $"{Name}に値をセット : {value}",
                        () => Targets.ApplyValue(result),
                        () => Targets.ForEach((target,i)=>target.Value = oldValues[i])
                    );
                }
            }
        }

        public string[] Items { get; init; }

    }


    public class MethodEditor : Inspectable
    {
        public IEnumerable<InspectableMethod> Targets { get; }
        public MethodEditor(IEnumerable<InspectableMethod> targets)
        {
            Name = targets.First().Name;
            Targets = targets;
            Command = new DelegateCommand(param =>
            {
                foreach (var p in targets)
                {
                    if (p.Command.CanExecute(param))
                    {
                        p.Command.Execute(param);
                    }
                }
            }, param => Targets.Any(p => p.Command.CanExecute(param)));
        }

        public string Name { get; }

        /// <summary>
        /// メソッドを実行するコマンド
        /// </summary>
        public ICommand Command { get; }

    }


    [PropertyEditor(typeof(bool))]
    public class BoolEditor : Editor
    {
        public BoolEditor(InspectableProperty[] targets) : base(targets)
        {
        }
        public bool? Value
        {
            get => IsSameValues ? Targets.FirstElement<bool>() : null;
            set
            {
                var oldValues = Targets.Select(i => i.Value).ToArray();
                History.History.Record(
                    $"{Name}に値をセット : {value}",
                    () => Targets.ApplyValue(value),
                    () => Targets.ForEach((target, i) => target.Value = oldValues[i])
                );
            }
        }
    }

    [PropertyEditor(typeof(string))]
    public class StringEditor : Editor
    {
        public StringEditor(InspectableProperty[] targets) : base(targets)
        {
        }
        public string Value
        {
            get => IsSameValues ? Targets.FirstElement<string>() : MultiValuesText;
            set
            {
                var oldValues = Targets.Select(i => i.Value).ToArray();
                History.History.Record(
                    $"{Name}に値をセット : {value}",
                    () => Targets.ApplyValue(value),
                    () => Targets.ForEach((target, i) => target.Value = oldValues[i])
                );
            }
        }
    }


    [PropertyEditor(typeof(sbyte), typeof(byte), typeof(short), typeof(ushort), typeof(int), typeof(uint), typeof(long), typeof(ulong))]
    public class ScalarEditor: Editor
    {
        public ScalarEditor(InspectableProperty[] targets) : base(targets)
        {
            _type = Targets.First().Type;
        }

        private void BeginEdit()
        {
            _oldValues = Targets.Select(i => i.Value).ToArray();
        }
        private void EndEdit()
        {
            if (double.TryParse(Value, out var v))
            {
                var newValue = Convert.ChangeType(v, _type);

                History.History.Record(
                    $"{Name}に値をセット : {newValue}",
                    () => Targets.ApplyValue(newValue),
                    () => Targets.ForEach((target, i) => target.Value = _oldValues![i])
                );
            }
        }

        public string Value
        {
            get => IsSameValues ? ((double?)Convert.ChangeType(Targets.First().Value,typeof(double))??0.0).ToString() : MultiValuesText;
            set
            {
                if(double.TryParse(value, out var v))
                {
                    var newValue = Convert.ChangeType(v, _type);
                    var oldValues = Targets.Select(i => i.Value).ToArray();
                    History.History.Record(
                        $"{Name}に値をセット : {newValue}",
                        () => Targets.ApplyValue(newValue),
                        () => Targets.ForEach((target, i) => target.Value = oldValues[i])
                    );
                }
            }
        }

        private object?[]? _oldValues;
        private Type _type;
    }


    [PropertyEditor(typeof(Vector2))]
    public class Vector2Editor : Editor
    {
        public Vector2Editor(InspectableProperty[] targets) : base(targets,[nameof(IsSameXValues), nameof(IsSameYValues)])
        {
        }

        public string X
        {
            get => IsSameXValues ? Targets.FirstElement<Vector2>().X.ToString() :MultiValuesText;
            set
            {
                if (float.TryParse(value, out var v))
                {
                    var oldValues = Targets.Select(i => i.Value).ToArray();
                    History.History.Record(
                        $"{Name}に値をセット : {v}",
                        () =>
                        {
                            foreach (var p in Targets)
                            {
                                if (p.Value is Vector2 vec)
                                {
                                    vec.X = v;
                                    p.Value = vec;
                                }
                            }
                        },
                        () => Targets.ForEach((target, i) => target.Value = oldValues[i])
                    );

                }
            }
        }
        public string Y
        {
            get => IsSameYValues ? Targets.FirstElement<Vector2>().Y.ToString() : MultiValuesText;
            set
            {
                if (float.TryParse(value, out var v))
                {
                    var oldValues = Targets.Select(i => i.Value).ToArray();
                    History.History.Record(
                        $"{Name}に値をセット : {v}",
                        () =>
                        {
                            foreach (var p in Targets)
                            {
                                if (p.Value is Vector2 vec)
                                {
                                    vec.Y = v;
                                    p.Value = vec;
                                }
                            }
                        },
                        () => Targets.ForEach((target, i) => target.Value = oldValues[i])
                    );

                }
            }
        }


        public bool IsSameXValues => Targets.Select(i=>i.Value).Cast<Vector2>().All(i => i.X == ((Vector2)Target.Value!).X);
        public bool IsSameYValues => Targets.Select(i=>i.Value).Cast<Vector2>().All(i => i.Y == ((Vector2)Target.Value!).Y);
    }


    [PropertyEditor(typeof(Vector3))]
    public class Vector3Editor : Editor
    {
        public Vector3Editor(InspectableProperty[] targets) : base(targets, [nameof(IsSameXValues), nameof(IsSameYValues), nameof(IsSameZValues)])
        {
        }

        public string X
        {
            get => IsSameXValues ? Targets.FirstElement<Vector3>().X.ToString() : MultiValuesText;
            set
            {
                if (float.TryParse(value, out var v))
                {
                    var oldValues = Targets.Select(i => i.Value).ToArray();
                    History.History.Record(
                        $"{Name}に値をセット : {v}",
                        () =>
                        {
                            foreach (var p in Targets)
                            {
                                if (p.Value is Vector3 vec)
                                {
                                    vec.X = v;
                                    p.Value = vec;
                                }
                            }
                        },
                        () => Targets.ForEach((target, i) => target.Value = oldValues[i])
                    );

                }
            }
        }
        public string Y
        {
            get => IsSameYValues ? Targets.FirstElement<Vector3>().Y.ToString() : MultiValuesText;
            set
            {
                {
                    if (float.TryParse(value, out var v))
                    {
                        var oldValues = Targets.Select(i => i.Value).ToArray();
                        History.History.Record(
                            $"{Name}に値をセット : {v}",
                            () =>
                            {
                                foreach (var p in Targets)
                                {
                                    if (p.Value is Vector3 vec)
                                    {
                                        vec.Y = v;
                                        p.Value = vec;
                                    }
                                }
                            },
                            () => Targets.ForEach((target, i) => target.Value = oldValues[i])
                        );

                    }
                }
            }
        }
        public string Z
        {
            get => IsSameZValues ? Targets.FirstElement<Vector3>().Z.ToString() : MultiValuesText;
            set
            {
                {
                    if (float.TryParse(value, out var v))
                    {
                        var oldValues = Targets.Select(i => i.Value).ToArray();
                        History.History.Record(
                            $"{Name}に値をセット : {v}",
                            () =>
                            {
                                foreach (var p in Targets)
                                {
                                    if (p.Value is Vector3 vec)
                                    {
                                        vec.Z = v;
                                        p.Value = vec;
                                    }
                                }
                            },
                            () => Targets.ForEach((target, i) => target.Value = oldValues[i])
                        );

                    }
                }
            }
        }

        public bool IsSameXValues => Targets.Select(i => i.Value).Cast<Vector3>().All(i => i.X == ((Vector3)Target.Value!).X);
        public bool IsSameYValues => Targets.Select(i => i.Value).Cast<Vector3>().All(i => i.Y == ((Vector3)Target.Value!).Y);
        public bool IsSameZValues => Targets.Select(i => i.Value).Cast<Vector3>().All(i => i.Z == ((Vector3)Target.Value!).Z);
    }


}

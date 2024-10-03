using Common.Attribute;
using Common.Linq;
using Common.Math;
using System;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;

namespace CommonView.Controls
{
    public class InspectorView : ItemsControl
    {
        public static readonly DependencyProperty IndentProperty = DependencyProperty.Register(
                nameof(Indent),
                typeof(double),
                typeof(InspectorView),
                new PropertyMetadata(0.0));

        public static readonly DependencyProperty LabelWidthProperty = DependencyProperty.Register(
                nameof(LabelWidth),
                typeof(GridLength),
                typeof(InspectorView),
                new PropertyMetadata());

        public static readonly DependencyProperty TargetProperty = DependencyProperty.Register(
                nameof(Target),
                typeof(object),
                typeof(InspectorView),
                new PropertyMetadata(null, new PropertyChangedCallback(OnTargetChanged)));

        public double Indent
        {
            get => (double)GetValue(IndentProperty);
            set => SetValue(IndentProperty, value);
        }

        public GridLength LabelWidth
        {
            get => (GridLength)GetValue(LabelWidthProperty);
            set => SetValue(LabelWidthProperty, value);
        }

        public object? Target
        {
            get => GetValue(TargetProperty);
            set => SetValue(TargetProperty, value);
        }

        static InspectorView()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(InspectorView), new FrameworkPropertyMetadata(typeof(InspectorView)));
        }
        private static void OnTargetChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {
            if (sender is not InspectorView view) return;

            view.Items.Clear();

            if ((e.NewValue is null)) return;

            var obj = e.NewValue;

            var type = e.NewValue.GetType();

            {
                var properties = type.GetProperties(BindingFlags.Instance | BindingFlags.Public);
                foreach (PropertyInfo p in properties)
                {
                    var space = p.GetCustomAttribute<SpaceAttribute>();
                    if (space!=null)
                    {
                        var spacer = new InspectorItemContainer();
                        spacer.Height = space.Space;
                        view.AddChild(spacer);
                    }

                    var container = new InspectorItemContainer();

                    var displayName = p.GetCustomAttribute<DisplayNameAttribute>();
                    container.Label = new TextBlock() { Text = displayName?.DisplayName??p.Name };

                    var tooltip = p.GetCustomAttribute<TooltipAttribute>();
                    container.ToolTip = tooltip?.Tooltip;

                    var hideLabel = p.GetCustomAttribute<HideLabelAttribute>()!=null;
                    container.CollapseLabel = hideLabel;

                    if (p.PropertyType == typeof(int))
                    {
                        container.Content = new TextBox() { Text = p.GetValue(obj)!.ToString() };
                    }
                    else if (p.PropertyType == typeof(float))
                    {
                        container.Content = new TextBox() { Text = p.GetValue(obj)!.ToString() };
                    }
                    else if (p.PropertyType == typeof(double))
                    {
                        container.Content = new TextBox() { Text = p.GetValue(obj)!.ToString() };
                    }
                    else if (p.PropertyType == typeof(string))
                    {
                        container.Content = new TextBox() { Text = (string)p.GetValue(obj)! };
                    }
                    else if (p.PropertyType == typeof(bool))
                    {
                        container.Content = new CheckBox() { IsChecked = (bool)p.GetValue(obj)! };
                    }
                    else if (p.PropertyType.IsEnum)
                    {
                        var comboBox = new ComboBox();
                        Enum.GetNames(p.PropertyType).ForEach(i=>comboBox.Items.Add(i));
                        comboBox.SelectedItem = p.GetValue(obj)!.ToString();
                        container.Content = comboBox;
                    }
                    else if (p.PropertyType == typeof(Vector3))
                    {
                        var value = (Vector3)p.GetValue(obj)!;

                        var grid = new Grid();
                        grid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
                        grid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
                        grid.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

                        grid.Children.Add(new TextBox() { Text= value.x.ToString(), Margin = new Thickness(0, 0, 1, 0) });
                        grid.Children.Add(new TextBox() { Text= value.y.ToString(), Margin = new Thickness(0, 0, 1, 0) });
                        grid.Children.Add(new TextBox() { Text= value.z.ToString(), Margin = new Thickness(0, 0, 0, 0) });

                        for (int i = 0; i<grid.Children.Count; ++i)
                        {
                            Grid.SetColumn(grid.Children[i], i);
                        }

                        container.Content = grid;
                    }else if(p.PropertyType.IsArray)
                    {
                        container.CollapseLabel = true;

                        var expander = new Expander() { Header = p.Name };
                        expander.Background = System.Windows.Media.Brushes.Transparent;

                        var listContainer = new InspectorView();
                        Grid.SetIsSharedSizeScope(listContainer, false);

                        var array = (System.Array)p.GetValue(obj)!;

                        for (int i = 0; i < array.Length; ++i)
                        {
                            var itemContaienr = new InspectorItemContainer();
                            itemContaienr.Label = $"{p.Name}[{i}]";
                            var value = array.GetValue(i);
                            var text = new TextBox() { Text = value?.ToString() };
                            itemContaienr.Content = text;
                            listContainer.AddChild(itemContaienr);
                        }

                        expander.Content = listContainer;
                        container.Content = expander;
                    }
                    else
                    {
                        container.Content = new TextBlock() { Text = p.GetValue(obj)!.ToString() };
                    }

                    if (!p.CanWrite)
                    {
                        if(container.Content is Control ctrl)
                        {
                            // ctrl.IsEnabled = false;
                        }
                    }

                    view.AddChild(container);
                }
            }

            {
                var methods = type.GetMethods(BindingFlags.Instance | BindingFlags.Public | BindingFlags.DeclaredOnly);
                foreach (MethodInfo m in methods)
                {
                    // setter,getter,add,removeなどの特殊なメソッドは無視
                    if (m.IsSpecialName) continue;
                    if (m.GetParameters().Length!=0) continue;

                    var container = new InspectorItemContainer();

                    var displayName = m.GetCustomAttribute<DisplayNameAttribute>();
                    container.Label = displayName?.DisplayName??m.Name;

                    var tooltip = m.GetCustomAttribute<TooltipAttribute>();
                    container.ToolTip = tooltip?.Tooltip;

                    var hideLabel = m.GetCustomAttribute<HideLabelAttribute>()!=null;
                    container.CollapseLabel = hideLabel;

                    var button = new Button() { Content = "実行" };
                    button.Click += (sender, e) => { m.Invoke(obj, null); };
                    container.Content = button;
                    view.AddChild(container);
                }
            }

        }
    }
}

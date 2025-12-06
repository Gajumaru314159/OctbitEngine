using Common.Attribute;
using Common.Math;
using System.Collections;
using System.Collections.ObjectModel;
using System.Reflection;
using System.Windows.Controls;
using System.Windows;
using Livet;

namespace CommonView.Controls.Inspector.Reflection
{
    public interface IEnum
    {
        string Value { get; set; }
        string[] Items { get; }
    }


    public class InspectableReflectionObject : InspectableObject
    {

        public static ObservableCollection<Inspectable> Create(object[] targets)
        {
            if (targets.Length==0) return new();

            var type = targets[0].GetType();

            if (EditorRegistory.FindViewModel(type) is Type objectVmType)
            {
                return CreateObject(targets);
            }
            else
            {
                return CreateProperties(targets);
            }
        }

        public static ObservableCollection<Inspectable> CreateObject(object[] targets)
        {
            var result = new ObservableCollection<Inspectable>();
            if (targets.Length==0) return result;

            var target = targets[0];
            var type = target.GetType();

            targets = targets.Where(i => i.GetType()==type).ToArray();
            if (targets.Length==0) return result;


            // EntityならばInspectableEntityを返したい
            if (EditorRegistory.FindViewModel(type) is Type objectVmType)
            {
                if (Activator.CreateInstance(objectVmType, [targets]) is Inspectable editor)
                {
                    result.Add(editor);
                }
            }

            return result;
        }


        public static ObservableCollection<Inspectable> CreateProperties(object[] targets)
        {
            var result = new ObservableCollection<Inspectable>();
            if (targets.Length==0) return result;

            var target = targets[0];
            var type = target.GetType();

            targets = targets.Where(i => i.GetType()==type).ToArray();
            if (targets.Length==0) return result;

            {
                var properties = type.GetProperties(BindingFlags.Instance | BindingFlags.Public);
                foreach (PropertyInfo p in properties)
                {
                    if (p.GetCustomAttribute<NotInspectableAttribute>()!=null) continue;

                    var vm = EditorRegistory.FindViewModel(p.PropertyType);
                    var irps = targets.Select(i => new InspectableReflectionProperty(i, p)).ToArray();

                    if (vm is not null)
                    {
                        if (Activator.CreateInstance(vm, [irps]) is Editor editor)
                        {
                            result.Add(editor);
                        }
                    }
                    else if (p.PropertyType.IsEnum)
                    {
                        result.Add(new EnumEditor(irps));
                    }
                    else if (p.PropertyType.IsSubclassOf(typeof(IEnum)))
                    {
                        //result.Add(new EnumEditor(irps));
                    }
                    else if (p.PropertyType.GetInterfaces().Contains(typeof(IList)))
                    {
                        result.Add(new InspectableReflectionList(p, targets));
                    }
                    else if (p.PropertyType.IsClass)
                    {
                        result.Add(new InspectableReflectionObject(p.Name, targets.Select(i => p.GetValue(i)!).ToArray()));
                    }

                }

                var methods = type.GetMethods(BindingFlags.Instance | BindingFlags.Public | BindingFlags.DeclaredOnly);
                foreach (MethodInfo p in methods)
                {
                    if (p.IsSpecialName) continue;
                    if (p.GetParameters().Length!=0) continue;
                    result.Add(new MethodEditor(targets.Select(i => new InspectableReflectionMethod(i, p)).ToArray()));
                }
            }

            return result;
        }

        internal InspectableReflectionObject(string name, object[] targets)
        {
            Name = name;
            _inspectables = Create(targets);
            Inspectables = new(_inspectables);
        }

        public override string Name { get; } = "-";
        public override ReadOnlyObservableCollection<Inspectable> Inspectables { get; }
        public ObservableCollection<Inspectable> _inspectables { get; } = new();
    }

}

using Common.Math;
using Reactive.Bindings.Extensions;
using System.Collections;
using System.Collections.ObjectModel;
using System.Reactive.Linq;
using System.Reflection;

namespace CommonView.Controls.Inspector.Reflection
{

    public class InspectableReflectionList : InspectableObject
    {
        public InspectableReflectionList(PropertyInfo propertyInfo, object[] targets)
        {
            Inspectables = new(_inspectables);
            Lists = targets.Select(i=>(IList)propertyInfo.GetValue(i)!).ToArray();
            PropertyInfo = propertyInfo;
                        
            m_countProperty = new ScalarEditor(Lists.Select(i => new InspectableReflectionListCountProperty(i)).ToArray());

            if(m_countProperty.Target is InspectableReflectionListCountProperty a)
            {
                Observable
                    .FromEvent(h => a.Resized+=h, h => a.Resized-=h)
                    .Subscribe(size=>OnResized())
                    .AddTo(CompositeDisposable);
            }
            _inspectables.Add(m_countProperty);

            GenerateInspectables();
        }

        private void GenerateInspectables()
        {
            var oldCount = Inspectables.Count -1;
            var newCount = Lists.Min(i=>i.Count);


            while (1<Inspectables.Count) _inspectables.RemoveAt(Inspectables.Count-1);


            var type = Lists[0].GetType();
            Type? elementType = null;
            if (type.IsArray) elementType = type.GetElementType()!;
            if (type.IsGenericType) elementType = type.GetGenericArguments()[0]!;
            if(elementType==null) return;

            var vm = EditorRegistory.FindViewModel(elementType);
            // TODO Enum対応

            for (int i = 0; i<newCount; i++)
            {
                if (vm!=null)
                {
                    var irps = Lists.Select(list => new InspectableReflectionListItem(list, PropertyInfo,i)).ToArray();
                    if (Activator.CreateInstance(vm, [irps]) is Editor editor)
                    {
                        _inspectables.Add(editor);
                    }
                }
                else
                {
                    _inspectables.Add(new InspectableReflectionObject($"[{i}]", [Lists[i]!]));
                }
            }

            RaisePropertyChanged(nameof(Inspectables));
        }

        private void OnResized()
        {
            GenerateInspectables();
        }

        public override string Name => PropertyInfo.Name;
        public ObservableCollection<Inspectable> _inspectables { get; } = new();
        public override ReadOnlyObservableCollection<Inspectable> Inspectables { get; }
        public IList[] Lists { get; }
        public PropertyInfo PropertyInfo { get; }

        private ScalarEditor? m_countProperty;
    }



    /// <summary>
    /// インスペクタ表示可能なリフレクションプロパティ
    /// </summary>
    internal class InspectableReflectionListCountProperty : InspectableProperty
    {
        public InspectableReflectionListCountProperty(IList owner)
        {
            Owner = owner;

            var type = owner.GetType();
            if (type.IsArray) ElementType = type.GetElementType()!;
            if (type.IsGenericType) ElementType = type.GetGenericArguments()[0]!;
            if (ElementType == null) throw new ArgumentException("要素の型が取得できません。");
        }

        public override string DisplayName => Name;
        public override string Name => "Count";
        public override Type Type => typeof(int);
        public override object? Value
        {
            get => List.Count;
            set
            {
                if (value?.GetType() != Type)
                {
                    if (int.TryParse(value?.ToString(), out int tmp))
                    {
                        value = tmp;
                    }
                    else
                    {
                        return;
                    }
                }
                if (!CanWrite) return;
                if (value == Value) return;

                return;

                // TODO リストの復元対応

                var oldValue = Value;

                int delta = (int)value! - (int)oldValue!;

                if (0<delta)
                {
                    var appendItems = new List<object?>();
                    for (int i = 0; i<delta; ++i)
                    {
                        appendItems.Add(Activator.CreateInstance(ElementType));
                    }

                    History.History.Record(
                        $"{Name}に値をセット : {value?.ToString()}",
                        () =>
                        {
                            appendItems.ForEach(i => List.Add(i));
                            RaisePropertyChanged();
                            Resized?.Invoke();
                        },
                        () =>
                        {
                            for (int i = 0; i<delta; ++i)
                            {
                                List.RemoveAt(List.Count-1);
                            }
                            RaisePropertyChanged();
                            Resized?.Invoke();
                        }
                    );
                }
                else
                {
                    var removeItems = new List<object?>();
                    for (int i = -delta; i<0; ++i)
                    {
                        removeItems.Add(List[List.Count+i]);
                    }

                    History.History.Record(
                        $"{Name}に値をセット : {value?.ToString()}",
                        () =>
                        {
                            for (int i = 0; i<-delta; ++i)
                            {
                                List.RemoveAt(List.Count-1);
                            }
                            RaisePropertyChanged();
                            Resized?.Invoke();
                        },
                        () =>
                        {
                            removeItems.ForEach(i => List.Add(i));
                            RaisePropertyChanged();
                            Resized?.Invoke();
                        }
                    );
                }

            }
        }

        public override bool CanRead => true;
        public override bool CanWrite => true && !List.IsFixedSize;
        public override IReadOnlyDictionary<string, string> Tags => m_tags;
        private Dictionary<string, string> m_tags = new();



        public IList Owner { get; }
        public IList List => (IList)Owner;
        public Type ElementType { get; init; }

        public event Action? Resized;

    }



    /// <summary>
    /// インスペクタ表示可能なリフレクションプロパティ
    /// </summary>
    internal class InspectableReflectionListItem : InspectableProperty
    {
        public InspectableReflectionListItem(IList owner, PropertyInfo indexerProperty, int index)
        {
            Owner = owner;
            PropertyInfo = indexerProperty;
            Index = index;

            var type = owner.GetType();
            if (type.IsArray) ElementType = type.GetElementType()!;
            if (type.IsGenericType) ElementType = type.GetGenericArguments()[0]!;
            if (ElementType == null) throw new ArgumentException("要素の型が取得できません。");
        }

        public override string DisplayName => Name;
        public override string Name => $"[{Index}]";
        public override Type Type => ElementType;
        private Type ElementType { get; }
        public override object? Value
        {
            get => List[Index];
            set
            {
                if (value?.GetType() != Type)
                {
                    if (value is string str)
                    {
                        double v = 0;
                        if (double.TryParse(str, out v))
                        {
                            value = v;
                        }
                        else
                        {
                            return;
                        }
                    }

                    try
                    {
                        value = Convert.ChangeType(value, Type);
                    }
                    catch
                    {
                        return;
                    }
                }
                if (value == Value) return;

                var oldValue = Value;
                History.History.Record(
                    $"{Name}に値をセット : {value?.ToString()}",
                    () => { List[Index] = value; RaisePropertyChanged(); },
                    () => { List[Index] = oldValue; RaisePropertyChanged(); }
                );
            }
        }

        public override bool CanRead => PropertyInfo.CanRead;
        public override bool CanWrite => PropertyInfo.CanWrite;
        public override IReadOnlyDictionary<string, string> Tags => m_tags;
        private Dictionary<string, string> m_tags = new();



        public IList List => (IList)Owner;

        public IList Owner { get; }
        public PropertyInfo PropertyInfo { get; }
        public int Index { get; }

    }
}

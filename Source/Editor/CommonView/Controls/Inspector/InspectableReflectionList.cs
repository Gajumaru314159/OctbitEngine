using Common.Attribute;
using Common.Log;
using Common.Math;
using System.Collections;
using System.Collections.ObjectModel;
using System.Reflection;

namespace CommonView.Controls.Inspector
{

    public class InspectableReflectionList : InspectableObject
    {
        public InspectableReflectionList(object owner, PropertyInfo propertyInfo)
        {
            List = (IList)propertyInfo.GetValue(owner)!;
            PropertyInfo = propertyInfo;

            m_countProperty = new InspectableReflectionListCountProperty(List);
            m_countProperty.Resized += OnResized;

            Inspectables.Add(m_countProperty);

            GenerateInspectables();
        }
        
        private void GenerateInspectables()
        {
            var count = List.Count;

            while (1<Inspectables.Count) Inspectables.RemoveAt(Inspectables.Count-1);

            for (int i = 0; i<count; i++)
            {
                Inspectables.Add(new InspectableReflectionListItem(List, PropertyInfo, i));
            }

            RaisePropertyChanged(nameof(Inspectables));
        }

        private void OnResized()
        {
            GenerateInspectables();
        }

        public override string Name => PropertyInfo.Name;
        public override ObservableCollection<Inspectable> Inspectables { get; } = new();
        public IList List { get; }
        public PropertyInfo PropertyInfo { get; }

        private InspectableReflectionListCountProperty? m_countProperty;
    }


    /// <summary>
    /// インスペクタ表示可能なリフレクションプロパティ
    /// </summary>
    internal class InspectableReflectionListCountProperty : InspectableProperty
    {
        public InspectableReflectionListCountProperty(object owner)
        {
            Owner = owner;

            var type = owner.GetType();
            if (type.IsArray) ElementType = type.GetElementType()!;
            if (type.IsGenericType) ElementType = type.GetGenericArguments()[0]!;
            if(ElementType == null) throw new ArgumentException("要素の型が取得できません。");
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
                    if(int.TryParse(value?.ToString(),out int tmp))
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

                // TODO リストの復元対応

                var oldValue = Value;

                int delta = (int)value! - (int)oldValue!;

                if (0<delta)
                {
                    History.History.Record(
                        $"{Name}に値をセット : {value?.ToString()}",
                        () =>
                        {
                            for (int i = 0; i<delta; ++i)
                            {
                                List.Add(Activator.CreateInstance(ElementType));
                            }
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
                            for (int i = 0; i<-delta; ++i)
                            {
                                List.Add(Activator.CreateInstance(ElementType));
                            }
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



        public object Owner { get; }
        public IList List => (IList)Owner;
        public Type ElementType { get; init; }

        public event Action? Resized;

    }



    /// <summary>
    /// インスペクタ表示可能なリフレクションプロパティ
    /// </summary>
    internal class InspectableReflectionListItem : InspectableProperty
    {
        public InspectableReflectionListItem(object owner, PropertyInfo indexerProperty, int index)
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
            get =>List[Index];
            set
            {
                if (value?.GetType() != Type)
                {
                    if(value is string str)
                    {
                        double v = 0;
                        if(double.TryParse(str, out v))
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

        public object Owner { get; }
        public PropertyInfo PropertyInfo { get; }
        public int Index { get; }

    }
}

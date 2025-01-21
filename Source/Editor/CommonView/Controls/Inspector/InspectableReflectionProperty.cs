using Common.Attribute;
using System.Reflection;

namespace CommonView.Controls.Inspector.Reflection
{

    /// <summary>
    /// インスペクタ表示可能なリフレクションプロパティ
    /// </summary>
    internal class InspectableReflectionProperty : InspectableProperty
    {
        public InspectableReflectionProperty(object owner, PropertyInfo propertyInfo)
        {
            Owner = owner;
            PropertyInfo = propertyInfo;

            if (Owner is System.ComponentModel.INotifyPropertyChanged npc)
            {
                // TODO 購読解除
                npc.PropertyChanged += (s, e) =>
                {
                    if (e.PropertyName == Name)
                    {
                        RaisePropertyChanged(nameof(Value));
                    }
                };
            }

            // NOTE プロパティごとに生成する必要はないのでメンバに持たなくてもよい？
            m_tags = propertyInfo.GetCustomAttributes<TagAttribute>().ToDictionary(i => i.Key, i => i.Value);

            if (PropertyInfo.ReflectedType?.IsSubclassOf(Owner.GetType())??false)
            {
                throw new ArgumentException("プロパティがオーナーの型に属していません。");
            }

        }

        public override string DisplayName => PropertyInfo.GetCustomAttribute<DisplayNameAttribute>()?.DisplayName ?? Name;
        public override string Name => PropertyInfo.Name;
        public override Type Type => PropertyInfo.PropertyType;
        public override object? Value
        {
            get => PropertyInfo.GetValue(Owner);
            set
            {
                // TODO 型変換の責務は各PropertyControlに任せる
                if (value?.GetType() != Type)
                {
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
                
                PropertyInfo.SetValue(Owner, value); 
                RaisePropertyChanged();

                // var oldValue = Value;
                // History.History.Record(
                //     $"{Name}に値をセット : {value?.ToString()}",
                //     () => { PropertyInfo.SetValue(Owner, value); RaisePropertyChanged(); },
                //     () => { PropertyInfo.SetValue(Owner, oldValue); RaisePropertyChanged(); }
                // );
            }
        }

        public override bool CanRead => PropertyInfo.CanRead;
        public override bool CanWrite => PropertyInfo.CanWrite;
        public override IReadOnlyDictionary<string, string> Tags => m_tags;
        private Dictionary<string, string> m_tags = new();


        public object Owner { get; }
        public PropertyInfo PropertyInfo { get; }

    }
}

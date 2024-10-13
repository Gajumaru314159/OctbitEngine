using Common.Attribute;
using Common.Math;
using CommonView.Menu;
using Livet.Commands;
using System.Reflection;
using System.Windows.Input;

namespace CommonView.Controls.Inspector
{

    /// <summary>
    /// インスペクタ表示可能なリフレクションプロパティ
    /// </summary>
    internal class InspectableReflectionMethod : InspectableMethod
    {
        public InspectableReflectionMethod(object owner, MethodInfo methodInfo)
        {
            Owner = owner;
            MethodInfo = methodInfo;
            Command = new DelegateCommand(() => MethodInfo.Invoke(Owner, null));

            // NOTE プロパティごとに生成する必要はないのでメンバに持たなくてもよい？
            m_tags = MethodInfo.GetCustomAttributes<TagAttribute>().ToDictionary(i => i.Key, i => i.Value);

            if (MethodInfo.ReflectedType?.IsSubclassOf(Owner.GetType())??false)
            {
                throw new ArgumentException("プロパティがオーナーの型に属していません。");
            }

        }

        public object Owner { get; }
        public override string DisplayName => MethodInfo.GetCustomAttribute<DisplayNameAttribute>()?.DisplayName ?? Name;
        public override string Name => MethodInfo.Name;
        public override ICommand Command { get; }

        public override IReadOnlyDictionary<string, string> Tags => m_tags;
        private Dictionary<string, string> m_tags = new();

        public MethodInfo MethodInfo { get; }

    }
}

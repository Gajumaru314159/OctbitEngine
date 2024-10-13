using System.Windows.Input;

namespace CommonView.Controls
{
    public abstract class InspectableMethod : Inspectable
    {
        /// <summary>
        /// インスペクタに表示する名前
        /// 表示言語によって変わることがあります。
        /// </summary>
        public abstract string DisplayName { get; }

        /// <summary>
        /// メソッド名
        /// </summary>
        public abstract string Name { get; }

        /// <summary>
        /// メソッドを実行するコマンド
        /// </summary>
        public abstract ICommand Command { get; }

        /// <summary>
        /// タグ
        /// </summary>
        public abstract IReadOnlyDictionary<string, string> Tags { get; }
    }

}

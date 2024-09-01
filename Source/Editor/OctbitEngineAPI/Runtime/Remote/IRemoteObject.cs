using System.Runtime.CompilerServices;

namespace OctbitEngine.Runtime
{
    public interface IRemoteObject
    {
        IRuntime Runtime { get; }

        ITypeInfo TypeInfo { get; }


        void Release();

        #region Properties

        /// <summary>
        /// プロパティの値を取得します。
        /// </summary>
        T GetValue<T>([CallerMemberName] string? name = null) where T : notnull;

        /// <summary>
        /// プロパティの値を設定します。
        /// </summary>
        void SetValue<T>(T value, [CallerMemberName] string? name = null) where T : notnull;

        #endregion

        #region Methods

        /// <summary>
        /// 戻り値のないメソッドを呼び出します
        /// </summary>
        void Invoke(string name, IReadOnlyList<object>? args = null);

        /// <summary>
        /// 戻り値のあるメソッドを呼び出します
        /// </summary>
        T Invoke<T>(string name, IReadOnlyList<object>? args = null);


        /// <summary>
        /// 戻り値のないメソッドを呼び出します(非同期版)
        /// </summary>
        void InvokeAsync(string name, IReadOnlyList<object>? args = null);

        /// <summary>
        /// 戻り値のあるメソッドを呼び出します(非同期版)
        /// </summary>
        void InvokeAsync<T>(string name, Action<T> result,IReadOnlyList<object>? args = null);

        #endregion


    }
}

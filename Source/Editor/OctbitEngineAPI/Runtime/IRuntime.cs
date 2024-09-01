namespace OctbitEngine.Runtime
{

    public interface IRuntime
    {
        IWorld[] Worlds { get; }

        event Action<IRuntime>? Crashed;

        event Action<IWorld>? WorldCreated;
        event Action<IWorld>? WorldReleased;
        void Send<T>(T query) where T : Query;
        void Send<T>(T query, Action<object> responce, TimeSpan? timeout = null) where T : Query;
        void SendAsync<T>(T query,Action<object> responce,TimeSpan? timeout = null) where T : Query;

        ITypeInfo? FindTypeInfo(string name);


    }
}

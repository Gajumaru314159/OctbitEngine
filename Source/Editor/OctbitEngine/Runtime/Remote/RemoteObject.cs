using System.Runtime.CompilerServices;

namespace OctbitEngine.Runtime
{

    [AttributeUsage(AttributeTargets.Class)]
    public class RuntimeAttribute : Attribute
    {
       public string Name { get; }
        public RuntimeAttribute(string name)
        {
            Name = name;
        }
    }


    public class RemoteObject : IRemoteObject
    {
        private UserTypeObject UserTypeObject { get; init; }

        //internal RemoteObject(IRuntime runtime, ITypeInfo typeInfo,int id)
        public RemoteObject(IRuntime runtime, ITypeInfo typeInfo,int id)
        {
            Runtime = runtime;
            TypeInfo = typeInfo;
            Id = id;
            UserTypeObject = new UserTypeObject(typeInfo);
        }
        public int Id { get; private init; }

        public IRuntime Runtime { get; }

        public ITypeInfo TypeInfo { get; }
        
        public object? GetValue([CallerMemberName] string? name = null)
        {
            return UserTypeObject.GetValue(name);
        }
        public void SetValue(object? value, [CallerMemberName] string? name = null)
        {
            UserTypeObject.SetValue(value, name);
            // TODO 値の転送
        }
        public T GetValue<T>([CallerMemberName] string? name = null) where T : notnull
        {
            return UserTypeObject.GetValue<T>(name);
        }
        public void SetValue<T>(T value, [CallerMemberName] string? name = null) where T : notnull
        {
            UserTypeObject.SetValue(value, name);
            // TODO 値の転送
        }

        public void Invoke(string name, IReadOnlyList<object>? args = null)
        {
            if(TypeInfo.FindMethod(name)==null)
            {
                throw new Exception($"メソッド{name}は存在しません");
            }

            var query = new InvokeRemoteObjectMethodQuery()
            {
                RemoteId = Id,
                Name = name,
            };

            Runtime.Send(query);
        }

        public T Invoke<T>(string name, IReadOnlyList<object>? args = null)
        {
            throw new NotImplementedException();
        }

        public void InvokeAsync(string name, IReadOnlyList<object>? args = null)
        {
            if (TypeInfo.FindMethod(name)==null)
            {
                throw new Exception($"メソッド{name}は存在しません");
            }

            var query = new InvokeRemoteObjectMethodQuery()
            {
                RemoteId = Id,
                Name = name,
            };

            Runtime.SendAsync(query);
        }

        public void InvokeAsync<T>(string name, Action<T> result, IReadOnlyList<object>? args = null)
        {
            throw new NotImplementedException();
        }

        public void Release()
        {
            throw new NotImplementedException();
        }

    }


}

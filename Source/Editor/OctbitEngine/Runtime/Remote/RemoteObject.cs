using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

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
        internal RemoteObject(IRuntime runtime, ITypeInfo typeInfo,int id)
        {
            Runtime = runtime;
            TypeInfo = typeInfo;
        }

        public IRuntime Runtime { get; }

        public ITypeInfo TypeInfo { get; }

        public T GetValue<T>([CallerMemberName] string? name = null) where T : notnull
        {
            throw new NotImplementedException();
        }

        public void Invoke(string name, IReadOnlyList<object>? args = null)
        {
            throw new NotImplementedException();
        }

        public T Invoke<T>(string name, IReadOnlyList<object>? args = null)
        {
            throw new NotImplementedException();
        }

        public void InvokeAsync(string name, IReadOnlyList<object>? args = null)
        {
            throw new NotImplementedException();
        }

        public void InvokeAsync<T>(string name, Action<T> result, IReadOnlyList<object>? args = null)
        {
            throw new NotImplementedException();
        }

        public void Release()
        {
            throw new NotImplementedException();
        }

        public void SetValue<T>(T value, [CallerMemberName] string? name = null) where T : notnull
        {
            throw new NotImplementedException();
        }
    }


}

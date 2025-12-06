using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.Runtime
{
    internal class RemoteObjectManager
    {
        private Dictionary<int, IRemoteObject> m_objects = new();

        public void CreateFromRuntime(int remoteId)
        {

        }

        public void CreateFromEditor(int remoteId, IRemoteObject remoteObject)
        {
            lock (m_objects)
            {
                m_objects.Add(remoteId, remoteObject);
            }
        }

        public IRemoteObject? Find(int remoteId)
        {
            lock (m_objects)
            {
                if(m_objects.TryGetValue(remoteId, out var remoteObject))
                {
                    return remoteObject;
                }
                return null;
            }
        }
    }
}

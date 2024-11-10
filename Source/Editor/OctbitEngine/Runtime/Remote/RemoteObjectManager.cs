using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.Runtime
{
    internal class RemoteObjectManager
    {
        private Dictionary<int, IRemoteObject> m_obects = new();

        public void CreateFromRuntime(int remoteId)
        {

        }

        public void CreateFromEditor(int remoteId, IRemoteObject remoteObject)
        {
            lock (m_obects)
            {
                m_obects.Add(remoteId, remoteObject);
            }
        }

        public IRemoteObject? Find(int remoteId)
        {
            lock (m_obects)
            {
                if(m_obects.TryGetValue(remoteId, out var remoteObject))
                {
                    return remoteObject;
                }
                return null;
            }
        }
    }
}

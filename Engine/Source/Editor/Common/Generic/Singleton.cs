using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Common.Generic
{
    public class Singleton<T> where T : new()
    {
        private static T? m_instance;
        private static object m_lock = new();

        protected Singleton()
        {
        }

        public static T Instance
        {
            get
            {
                if (m_instance == null)
                {
                    lock (m_lock)
                    {
                        if (m_instance == null)
                        {
                            m_instance = new T();
                        }
                    }
                }
                return m_instance;
            }
        }
    }
}

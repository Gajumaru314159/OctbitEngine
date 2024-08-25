using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.Config
{
    public class WorkSpace
    {
        private static string RootMark = "EngineRootMark";
        static WorkSpace()
        {
            m_rootPath = Directory.GetCurrentDirectory();

            while (true)
            {
                var parent = Directory.GetParent(RootPath);
                if (parent != null)
                {
                    m_rootPath = parent.FullName;
                    if (File.Exists(Path.Combine(parent.FullName, RootMark)))
                    {
                        break;
                    }
                }

            }
        }

        private static string m_rootPath;
        public static string RootPath => m_rootPath;
    }
}

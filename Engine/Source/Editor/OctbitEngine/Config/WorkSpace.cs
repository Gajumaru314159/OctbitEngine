namespace OctbitEngine.Config
{
    public class WorkSpace
    {
        private static string RootMark = "EngineRootMark";
        static WorkSpace()
        {
            m_rootPath = Directory.GetCurrentDirectory()+"/a";


            bool hasRootMark = false;
            while (true)
            {
                var parent = Directory.GetParent(RootPath);
                if (parent != null)
                {
                    m_rootPath = parent.FullName;
                    if (File.Exists(Path.Combine(parent.FullName, RootMark)))
                    {
                        hasRootMark = true;
                        break;
                    }
                }
                else
                {
                    break;
                }

            }

            if (hasRootMark == false)
            {
                m_rootPath = Directory.GetCurrentDirectory();
            }
        }

        private static string m_rootPath;
        public static string RootPath => m_rootPath;
        public static string AssetsPath => Path.Combine(RootPath,"Assets");
        public static string BinariesPath => Path.Combine(RootPath,"Binaries");
        public static string CachePath => Path.Combine(RootPath, "Cache");
    }
}

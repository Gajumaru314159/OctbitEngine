using Common.Math;

namespace OctbitEngine.Runtime
{
    
    public static class TypeInfoUtility
    {
        private static Dictionary<string, Type> s_builtinTypeMap = new();
        static TypeInfoUtility()
        {
            void Add(string name)
            {
                var type = Type.GetType(name)??typeof(Vector2).Assembly.GetType(name);
                if (type == null) throw new ArgumentException($"{name}がアセンブリ内に存在しません");
                s_builtinTypeMap.Add(name, type);
            }
            Add("System.Int64");
            Add("System.Int32");
            Add("System.Int16");
            Add("System.SByte");
            Add("System.UInt64");
            Add("System.UInt32");
            Add("System.UInt16");
            Add("System.Byte");
            Add("System.Double");
            Add("System.Single");
            Add("System.Char");
            Add("System.Boolean");
            Add("System.String");
            Add("Common.Math.Vector2");
            Add("Common.Math.Vector3");
            Add("Common.Math.Vector4");
            Add("Common.Math.Rotation");
            Add("Common.Math.Quaternion");
            Add("Common.Graphics.Color");
        }

        public static bool IsBuiltinType(this ITypeInfo info)
        {
            if (info.Tags is null) return false;
            if (!info.Tags.TryGetValue("C#", out var value)) return false;
            return s_builtinTypeMap.ContainsKey(value);
        }
        public static bool TryGetBuiltinType(this ITypeInfo info,out Type? type)
        {
            type =null;
            if (info.Tags is null) return false;
            if (!info.Tags.TryGetValue("C#", out var value)) return false;
            return s_builtinTypeMap.TryGetValue(value,out type);
        }
    }

}

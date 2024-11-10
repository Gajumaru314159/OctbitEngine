namespace OctbitEngine.Runtime
{
    
    public static class TypeInfoUtility
    {
        private static Dictionary<string, Type> s_builtinTypeMap = new();
        static TypeInfoUtility()
        {
            void Add(string name) => s_builtinTypeMap.Add(name, Type.GetType(name)!);
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

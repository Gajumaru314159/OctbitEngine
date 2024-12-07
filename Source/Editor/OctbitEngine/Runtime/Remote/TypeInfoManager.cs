using Common.Log;
using Common.Linq;
using System.Text.Json;

namespace OctbitEngine.Runtime
{
    public struct EnumElementInfoArchive
    {
        public Dictionary<string, string> Tags { get; set; }
        public string Name { get; set; }
        public int Index { get; set; }
        public int Value { get; set; }
    }
    public struct ArgumntInfoArchive
    {
        public Dictionary<string, string> Tags { get; set; }
        public string Name { get; set; }
        public string Type { get; set; }
    }
    public struct ConstructorInfoArchive
    {
        public Dictionary<string, string> Tags { get; set; }
        public ArgumntInfoArchive[] Arguments { get; set; }
    }
    public struct PropertyInfoArchive
    {
        public Dictionary<string, string> Tags { get; set; }
        public string Name { get; set; }
        public string Type { get; set; }
        public bool CanRead { get; set; }
        public bool CanWrite { get; set; }
    }
    public struct MethodInfoArchive
    {
        public Dictionary<string, string> Tags { get; set; }
        public string Name { get; set; }
        public string ReturnType { get; set; }
        public ArgumntInfoArchive[] Arguments { get; set; }
    }
    public struct TypeInfoArchive
    {
        public Dictionary<string, string> Tags { get; set; }
        public string Name{ get; set; }
        public string[] Bases { get; set; }
        public ConstructorInfoArchive[] Constructors { get; set; }
        public PropertyInfoArchive[] Properties { get; set; }
        public MethodInfoArchive[] Methods { get; set; }
        public string[] PropertyOrder { get; set; }
        public string[] MethodOrder { get; set; }
    }



    internal class TypeInfoManager
    {
        internal TypeInfoManager(string json)
        {
            try
            {
                var archives = JsonSerializer.Deserialize<TypeInfoArchive[]>(json) ?? Array.Empty<TypeInfoArchive>();

                foreach (var archive in archives.NotNull())
                {
                    Type? type = null;
                    if (archive.Tags?.TryGetValue("C#", out var csName)??false)
                    {
                        // TODO 他アセンブリ対応
                        type = Type.GetType(csName);
                    }

                    TypeMap.Add(archive.Name, new TypeInfo(type??typeof(IUserTypeObject),archive.Name,archive.Tags??new()));
                }

                foreach (var archive in archives.NotNull())
                {
                    var typeInfo = TypeMap[archive.Name];

                    // ベースクラス
                    foreach (var baseName in archive.Bases.NotNull())
                    {
                        if (TypeMap.TryGetValue(baseName, out var baseType))
                        {
                            typeInfo.m_bases.Add(baseType);
                        }
                        else
                        {
                            // TODO 例外処理
                        }
                    }

                    // コンストラクタ
                    foreach (var constructorArchive in archive.Constructors.NotNull())
                    {
                        var arguments = new List<ArgumentInfo>();
                        foreach (var argArchive in constructorArchive.Arguments.NotNull())
                        {
                            if (TypeMap.TryGetValue(argArchive.Type, out var argType))
                            {
                                arguments.Add(new ArgumentInfo(argType, argArchive.Name,argArchive.Tags??new()));
                            }
                            else
                            {
                                // TODO 例外処理
                            }
                        }
                        typeInfo.m_constructors.Add(new ConstructorInfo(arguments,constructorArchive.Tags??new()));
                    }

                    // プロパティ
                    foreach (var propertyArchive in archive.Properties.NotNull())
                    {
                        if (TypeMap.TryGetValue(propertyArchive.Type, out var propertyType))
                        {
                            typeInfo.m_properties.Add(propertyArchive.Name, new PropertyInfo(propertyType, propertyArchive.Name, propertyArchive.CanRead, propertyArchive.CanWrite,propertyArchive.Tags??new()));
                        }
                        else
                        {
                            // TODO 例外処理
                        }
                    }

                    // メソッド
                    foreach (var methodArchive in archive.Methods.NotNull())
                    {
                        if (TypeMap.TryGetValue(methodArchive.ReturnType, out var returnType))
                        {
                            var arguments = new List<ArgumentInfo>();
                            foreach (var argArchive in methodArchive.Arguments.NotNull())
                            {
                                if (TypeMap.TryGetValue(argArchive.Type, out var argType))
                                {
                                    arguments.Add(new ArgumentInfo(argType, argArchive.Name,argArchive.Tags??new()));
                                }
                                else
                                {
                                    // TODO 例外処理
                                }
                            }
                            typeInfo.m_methods.Add(methodArchive.Name, new MethodInfo(methodArchive.Name, returnType, arguments,methodArchive.Tags??new()));
                        }
                        else
                        {
                            // TODO 例外処理
                        }
                    }

                    typeInfo.m_propertyOrder = archive.Properties.NotNull().Select(p => p.Name).ToList();
                    typeInfo.m_methodOrder = archive.Methods.NotNull().Select(m => m.Name).ToList();

                }
            }catch (Exception e)
            {
                Log.Error(e.Message);
            }

        }

        public ITypeInfo? Find(string type)
        {
            if (TypeMap.TryGetValue(type, out var typeInfo))
            {
                return typeInfo;
            }
            return null;
        }

        public Dictionary<string,TypeInfo> TypeMap { get; init; } = new();
    }
}

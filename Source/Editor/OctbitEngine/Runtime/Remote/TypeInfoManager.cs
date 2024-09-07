using System.Text.Json;

namespace OctbitEngine.Runtime
{
    public struct EnumElementInfoArchive
    {
        public string Name;
        public int Index;
        public int Value;
    }

    public struct ArgumntInfoArchive
    {
        public string Name;
        public string Type;
    }

    public struct ConstructorInfoArchive
    {
        public ArgumntInfoArchive[] Arguments;
    }

    public struct PropertyInfoArchive
    {
        public string Name;
        public string Type;
        public bool CanRead;
        public bool CanWrite;
    }

    public struct MethodInfoArchive
    {
        public string Name;
        public string ReturnType;
        public ArgumntInfoArchive[] Arguments;
    }


    public struct TypeInfoArchive
    {
        public string Name;
        public string[] Bases;
        public ConstructorInfoArchive[] Constructors;
        public PropertyInfoArchive[] Properties;
        public MethodInfoArchive[] Methods;
    }



    internal class TypeInfoManager
    {
        TypeInfoManager(string json)
        {
            var archives = JsonSerializer.Deserialize<TypeInfoArchive[]>(json) ?? Array.Empty<TypeInfoArchive>();

            var types = new Dictionary<string, TypeInfo>();

            foreach(var archive in archives)
            {
                types.Add(archive.Name, new TypeInfo(archive.Name));
            }

            foreach (var archive in archives)
            {
                var typeInfo = types[archive.Name];

                // ベースクラス
                foreach (var baseName in archive.Bases)
                {
                    if (types.TryGetValue(baseName, out var baseType))
                    {
                        typeInfo.m_bases.Add(baseType);
                    }
                    else
                    {
                        // TODO 例外処理
                    }
                }

                // コンストラクタ
                foreach (var constructorArchive in archive.Constructors)
                {
                    var arguments = new List<ArgumentInfo>();
                    foreach (var argArchive in constructorArchive.Arguments)
                    {
                        if (types.TryGetValue(argArchive.Type, out var argType))
                        {
                            arguments.Add(new ArgumentInfo(argType, argArchive.Name));
                        }
                        else
                        {
                            // TODO 例外処理
                        }
                    }
                    typeInfo.m_constructors.Add(new ConstructorInfo(arguments));
                }

                // プロパティ
                foreach (var propertyArchive in archive.Properties)
                {
                    if (types.TryGetValue(propertyArchive.Type, out var propertyType))
                    {
                        typeInfo.m_properties.Add(propertyArchive.Name, new PropertyInfo(propertyType, propertyArchive.Name, propertyArchive.CanRead, propertyArchive.CanWrite));
                    }
                    else
                    {
                        // TODO 例外処理
                    }
                }

                // メソッド
                foreach (var methodArchive in archive.Methods)
                {
                    if (types.TryGetValue(methodArchive.ReturnType, out var returnType))
                    {
                        var arguments = new List<ArgumentInfo>();
                        foreach (var argArchive in methodArchive.Arguments)
                        {
                            if (types.TryGetValue(argArchive.Type, out var argType))
                            {
                                arguments.Add(new ArgumentInfo(argType, argArchive.Name));
                            }
                            else
                            {
                                // TODO 例外処理
                            }
                        }
                        typeInfo.m_methods.Add(methodArchive.Name, new MethodInfo(methodArchive.Name, returnType, arguments));
                    }
                    else
                    {
                        // TODO 例外処理
                    }
                }

                typeInfo.m_propertyOrder = archive.Properties.Select(p => p.Name).ToList();
                typeInfo.m_methodOrder = archive.Methods.Select(m => m.Name).ToList();

            }


        }

        public static Dictionary<string,TypeInfo> TypeMap = new Dictionary<string, TypeInfo>();
    }
}

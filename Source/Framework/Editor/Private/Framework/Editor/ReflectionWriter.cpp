//***********************************************************
//! @file
//! @brief		ワールド
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Editor/ReflectionWriter.h>

#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <fstream>
#pragma warning(push,0)
#include <nlohmann/json.hpp>
#pragma warning(pop)

namespace ob::editor {
	
	static nlohmann::json CreateTags(const HashMap<StringView, String>& tags) {
		nlohmann::json out;
		for (auto& [key, value] : tags) {
			out[key] = value;
		}
		return out;
	}
	static nlohmann::json CreateTypes(const HashSet<Type>& types) {
		nlohmann::json out;
		for (auto& type : types) {
			out.emplace_back(type.name());
		}
		return out;
	}

	static nlohmann::json CreateArgument(const ArgumentInfo& arg) {
		nlohmann::json out;
		out["Name"] = arg.name;
		out["Type"] = arg.type.name();
		return out;
	}
	static nlohmann::json CreateArguments(const Vector<ArgumentInfo>& args) {
		nlohmann::json out;
		for (auto& arg : args) out.emplace_back(CreateArgument(arg));
		return out;
	}

	static nlohmann::json CreateConstructor(const ConstructorInfo& ctor) {
		nlohmann::json out;
		out["Tags"] = CreateTags(ctor.tags);
		auto& args = out["Argumets"];
		for (auto& arg : ctor.arguments) args.emplace_back(CreateArgument(arg));
		return out;
	}
	static nlohmann::json CreateConstructors(const Vector<ConstructorInfo>& ctors) {
		nlohmann::json out;
		for (auto& ctor : ctors) out.emplace_back(CreateConstructor(ctor));
		return out;
	}

	static nlohmann::json CreateProperty(const PropertyInfo& prop) {
		nlohmann::json out;
		out["Tags"] = CreateTags(prop.tags);
		out["Name"] = prop.name;
		out["Type"] = prop.type.name();
		out["CanRead"] = prop.canRead();
		out["CanWrite"] = prop.canWrite();
		return out;
	}
	static nlohmann::json CreateProperties(const PropertyInfoMap& props) {
		nlohmann::json out;
		for (auto& [name, prop] : props) out.emplace_back(CreateProperty(prop));
		return out;
	}

	static nlohmann::json CreateMethod(const MethodInfo& method) {
		nlohmann::json out;
		out["Tags"] = CreateTags(method.tags);
		out["Name"] = method.name;
		out["ReturnType"] = method.returnType.name();
		out["Arguments"] = CreateArguments(method.arguments);
		return out;
	}
	static nlohmann::json CreateMethods(const MethodInfoMap& methods) {
		nlohmann::json out;
		for (auto& [name, method] : methods) out.emplace_back(CreateMethod(method));
		return out;
	}

	static nlohmann::json CreateEnumElement(const EnumElementInfo& element) {
		nlohmann::json out;
		out["Name"] = element.name;
		out["Index"] = element.index;
		out["Value"] = element.value;
		return out;
	}
	static nlohmann::json CreateEnumElements(const Vector<EnumElementInfo>& elements) {
		nlohmann::json out;
		for (auto& element : elements) out.emplace_back(CreateEnumElement(element));
		return out;
	}


	nlohmann::json CreateTypeInfo(const TypeInfo& typeInfo) {

		nlohmann::json type;

		type["Tags"] = CreateTags(typeInfo.tags);
		type["Name"] = typeInfo.type.name();
		type["Bases"] = CreateTypes(typeInfo.bases);
		type["Constructors"] = CreateConstructors(typeInfo.constructors);
		type["Properties"] = CreateProperties(typeInfo.properties);
		type["Methods"] = CreateMethods(typeInfo.methods);
		type["PropertyOrder"] = typeInfo.propertyOrder;
		type["MethodOrder"] = typeInfo.methodOrder;

		type["IsEnum"] = typeInfo.isEnum;
		type["EnumElements"] = CreateEnumElements(typeInfo.enumElements);

		return type;
	}


	void ReflectionWriter::output(StringView path){

		nlohmann::json json;

		TypeInfoManager::Visit(
			[&](const TypeInfo& typeInfo) {
				json.push_back(CreateTypeInfo(typeInfo));
			}
		);

		std::ofstream o(path.data());
		o << std::setw(4) << json << std::endl;

	}

}

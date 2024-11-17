//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/String/String.h>
#include <Framework/Core/String/Format.h>
#include <Framework/Core/Reflection/TypeInfo.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief  TypeInfoを文字列化する
	//@―---------------------------------------------------------------------------
	inline String PrintTypeInfo(const TypeInfo& info) {

		String str;

		if (info.isEnum) {
			str = Format("enum {}", info.type.shortName());
			if (info.bases.empty() == false) {
				str += ":";
				for (auto& base : info.bases) {
					str += Format(" {},", base.shortName());
				}
				str.pop_back(1);
			}

			str += "{";

			for (auto& e : info.enumElements) {
				str += Format("\n    {},",e.name);
			}

			if (!info.enumElements.empty()) {
				str.pop_back();
			}

			str += "\n};";

		}
		else {

			str = Format("class {} ", info.type.shortName());
			if (info.bases.empty() == false) {
				str += ": ";
				for (auto& base : info.bases) {
					str += Format("public {},", base.shortName());
				}
				str.pop_back(1);
			}
			str += "{\n";

			// コンストラクタ
			if (!info.constructors.empty()) {
				str += "public:\n";
			}
			for (auto& constructor : info.constructors) {
				str += Format("    {}(", info.type.shortName());
				for (auto& arg : constructor.arguments) {
					str += Format("{} ", arg.type.shortName());
					str += Format("{},", arg.name);
				}
				if (constructor.arguments.empty() == false) {
					str.pop_back();
				}
				str += ");\n";
			}

			// メソッド
			if (!info.methods.empty()) {
				str += "public:\n";
			}
			for (auto& [name, method] : info.methods) {
				str += Format("    {} {}(", method.returnType.shortName(), method.name);
				for (auto& arg : method.arguments) {
					str += Format("{} ", arg.type.shortName());
					str += Format("{},", arg.name);
				}
				if (method.arguments.empty() == false) {
					str.pop_back();
				}
				str += ");\n";
			}

			// プロパティ
			if (!info.properties.empty()) {
				str += "public:\n";
			}
			for (auto& [name, property] : info.properties) {
				str += "    ";
				if (!property.canWrite()) str += "const ";
				str += Format("{} {};", property.type.shortName(), name);
				str += "\n";
			}

			str += "};";

		}

		return str;
	}

}

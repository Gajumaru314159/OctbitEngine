//***********************************************************
//! @file
//! @brief		プロファイラ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Core.h>
#include <Framework/Core/Reflection/Any.h>
#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Core/Log/Logger.h>
#include <Plugins/ImGui/ImGui.h>

namespace ob::debug {


	class Inspector {
	public:
		Inspector();

		void draw(Any obj) {

			TypeInfoManager manager;

			String name;
			std::string str;

			if (auto type = manager.find(obj.type())) {

				for (auto& [n, p] : type->properties) {

					name = n;

					if (p.type.is<bool>()) {
						auto value = p.get<bool>(obj);
						if (ImGui::Checkbox(name.c_str(), &value)) {
							p.set(obj, value);
						}
					}
					if (p.type.is<s32>()) {
						auto value = p.get<s32>(obj);
						if (ImGui::InputInt(name.c_str(), &value)) {
							p.set(obj, value);
						}
					}
					if (p.type.is<f32>()) {
						auto value = p.get<f32>(obj);
						if (ImGui::InputFloat(name.c_str(), &value)) {
							p.set(obj, value);
						}
					}
					if (p.type.is<String>()) {
						str = p.get<String>(obj);
						if (ImGui::InputText(name.c_str(), &str)) {
							p.set<String>(obj, str);
						}
					}

				}

			}

		}

	};

}
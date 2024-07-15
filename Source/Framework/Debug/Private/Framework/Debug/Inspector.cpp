//***********************************************************
//! @file
//! @brief		プロファイラ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Debug/Inspector.h>
#include <Plugins/ImGui/ImGui.h>

namespace ob::debug {

	void Inspector::draw(AnyReference obj, Type type) const {

		String name;
		std::string str;

		if (auto info = TypeInfoManager::Find(type)) {

			for (auto& [n, p] : info->properties) {

				name = n;

				if (p.type.is<bool>()) {
					auto value = p.get<bool>(obj);
					if (ImGui::Checkbox(name.c_str(), &value)) {
						p.setter((AnyReference)obj, value);
					}
				}
				if (p.type.is<s32>()) {
					auto value = p.get<s32>(obj);
					if (ImGui::InputInt(name.c_str(), &value)) {
						p.setter((AnyReference)obj, value);
					}
				}
				if (p.type.is<f32>()) {
					auto value = p.get<f32>(obj);
					if (ImGui::InputFloat(name.c_str(), &value)) {
						p.setter((AnyReference)obj, value);
					}
				}
				if (p.type.is<Vec3>()) {
					auto value = p.get<Vec3>(obj);
					if (ImGui::InputFloat3(name.c_str(), reinterpret_cast<f32*>(&value))) {
						p.setter((AnyReference)obj, value);
					}
				}
				if (p.type.is<Rot>()) {
					auto value = p.get<Rot>(obj);
					if (ImGui::InputFloat3(name.c_str(), reinterpret_cast<f32*>(&value))) {
						p.setter((AnyReference)obj, value);
					}
				}
				if (p.type.is<String>()) {
					str = p.get<String>(obj);
					if (ImGui::InputText(name.c_str(), &str)) {
						p.setter((AnyReference)obj, String(str));
					}
				}

			}

		}

	}

};
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Debug/Inspector.h>
#include <Plugins/ImGui/ImGui.h>

namespace ob::debug {

	void Inspector::draw(Any& obj, Type type) const {

		String name;
		std::string str;

		if (auto info = TypeInfoManager::Find(type)) {

			for (auto& namesv : info->propertyOrder) {

				name = namesv;
				auto& p = *info->findProperty(name);

				char format[32] = "%.3f";
				if (auto tag = p.findTag("Unit")) std::snprintf(format, sizeof(format), "%%.3f %s", tag->data());

				if (!p.canRead())continue;
				if (!p.canWrite()) ImGui::BeginDisabled();
				
				if (p.type.is<bool>()) {
					auto value = p.get<bool>(obj);
					if (ImGui::Checkbox(name.c_str(), &value)) {
						p.setter(obj, value);
					}
				} else if (p.type.is<s32>()) {
					auto value = p.get<s32>(obj);
					Optional<s32> min, max;
					if (auto tag = p.findTag("Min")) min = (s32)std::atoi(tag->data());
					if (auto tag = p.findTag("Max")) max = (s32)std::atoi(tag->data());
					if (min && max) {
						if (ImGui::SliderInt(name.c_str(), &value, min.value(), max.value())) {
							p.setter(obj, value);
						}
					} else {
						if (ImGui::DragInt(name.c_str(), &value)) {
							p.setter(obj, value);
						}
					}
				} else if (p.type.is<s16>()) {
					s32 value = p.get<s16>(obj);
					Optional<s32> min, max;
					if (auto tag = p.findTag("Min")) min = (s32)std::atoi(tag->data());
					if (auto tag = p.findTag("Max")) max = (s32)std::atoi(tag->data());
					if (min && max) {
						if (ImGui::SliderInt(name.c_str(), &value, min.value(), max.value())) {
							s16 value2 = value;
							p.setter(obj, value2);
						}
					} else {
						if (ImGui::DragInt(name.c_str(), &value)) {
							s16 value2 = value;
							p.setter(obj, value2);
						}
					}
				} else if (p.type.is<s8>()) {
					s32 value = p.get<s8>(obj);
					Optional<s32> min, max;
					if (auto tag = p.findTag("Min")) min = (s32)std::atoi(tag->data());
					if (auto tag = p.findTag("Max")) max = (s32)std::atoi(tag->data());
					if (min && max) {
						if (ImGui::SliderInt(name.c_str(), &value, min.value(), max.value())) {
							s8 value2 = value;
							p.setter(obj, value2);
						}
					} else {
						if (ImGui::DragInt(name.c_str(), &value)) {
							s8 value2 = value;
							p.setter(obj, value2);
						}
					}
				} else if (p.type.is<u32>()) {					
					s32 value = p.get<u32>(obj);
					Optional<s32> min, max;
					if (auto tag = p.findTag("Min")) min = (s32)std::atoi(tag->data());
					if (auto tag = p.findTag("Max")) max = (s32)std::atoi(tag->data());
					if (min && max) {
						if (ImGui::SliderInt(name.c_str(), &value, min.value(), max.value())) {
							u32 value2 = value;
							p.setter(obj, value2);
						}
					} else {
						if (ImGui::DragInt(name.c_str(), &value)) {
							u32 value2 = value;
							p.setter(obj, value2);
						}
					}
				} else if (p.type.is<u16>()) {
					s32 value = p.get<u16>(obj);
					Optional<s32> min, max;
					if (auto tag = p.findTag("Min")) min = (s32)std::atoi(tag->data());
					if (auto tag = p.findTag("Max")) max = (s32)std::atoi(tag->data());
					if (min && max) {
						if (ImGui::SliderInt(name.c_str(), &value, min.value(), max.value())) {
							u16 value2 = value;
							p.setter(obj, value2);
						}
					} else {
						if (ImGui::DragInt(name.c_str(), &value)) {
							u16 value2 = value;
							p.setter(obj, value2);
						}
					}
				} else if (p.type.is<u8>()) {
					s32 value = p.get<u8>(obj);
					Optional<s32> min, max;
					if (auto tag = p.findTag("Min")) min = (s32)std::atoi(tag->data());
					if (auto tag = p.findTag("Max")) max = (s32)std::atoi(tag->data());
					if (min && max) {
						if (ImGui::SliderInt(name.c_str(), &value, min.value(), max.value())) {
							u8 value2 = value;
							p.setter(obj, value2);
						}
					} else {
						if (ImGui::DragInt(name.c_str(), &value)) {
							u8 value2 = value;
							p.setter(obj, value2);
						}
					}
				} else if (p.type.is<f32>()) {
					auto value = p.get<f32>(obj);
					Optional<f32> min,max;
					if (auto tag = p.findTag("Min")) min = (f32)std::atof(tag->data());
					if (auto tag = p.findTag("Max")) max = (f32)std::atof(tag->data());

					if (min && max) {
						if (ImGui::SliderFloat(name.c_str(), &value,min.value(),max.value(), format)) {
							p.setter(obj, value);
						}
					} else {
						if (ImGui::DragFloat(name.c_str(), &value,1,0,0,format)) {
							p.setter(obj, value);
						}
					}
				} else if (p.type.is<f64>()) {
					f32 value = p.get<f64>(obj);
					Optional<f32> min,max;
					if (auto tag = p.findTag("Min")) min = (f32)std::atof(tag->data());
					if (auto tag = p.findTag("Max")) max = (f32)std::atof(tag->data());
					if (min && max) {
						if (ImGui::SliderFloat(name.c_str(), &value,min.value(),max.value(),format)) {
							f64 value2 = value;
							p.setter(obj, value2);
						}
					} else {
						if (ImGui::DragFloat(name.c_str(), &value,1,0,0,format)) {
							f64 value2 = value;
							p.setter(obj, value2);
						}
					}
				} else if (p.type.is<Vec2>()) {
					auto value = p.get<Vec2>(obj);
					if (ImGui::DragFloat2(name.c_str(), value,1,0,0,format)) {
						p.setter(obj, value);
					}
				} else if (p.type.is<Vec3>()) {
					auto value = p.get<Vec3>(obj);
					if (ImGui::DragFloat3(name.c_str(), value, 1, 0, 0, format)) {
						p.setter(obj, value);
					}
				} else if (p.type.is<Vec4>()) {
					auto value = p.get<Vec4>(obj);
					if (ImGui::DragFloat4(name.c_str(), value, 1, 0, 0, format)) {
						p.setter(obj, value);
					}
				} else if (p.type.is<IntVec2>()) {
					auto value = p.get<IntVec2>(obj);
					if (ImGui::DragInt2(name.c_str(), reinterpret_cast<s32*>(&value))) {
						p.setter(obj, value);
					}
				} else if (p.type.is<IntVec3>()) {
					auto value = p.get<IntVec3>(obj);
					if (ImGui::DragInt3(name.c_str(), reinterpret_cast<s32*>(&value))) {
						p.setter(obj, value);
					}
				} else if (p.type.is<IntVec4>()) {
					auto value = p.get<IntVec4>(obj);
					if (ImGui::DragInt4(name.c_str(), reinterpret_cast<s32*>(&value))) {
						p.setter(obj, value);
					}
				} else if (p.type.is<Rot>()) {
					auto value = p.get<Rot>(obj).toVec3();
					if (Math::IsNearZero(value.x)) value.x = 0.0f;
					if (Math::IsNearZero(value.y)) value.y = 0.0f;
					if (Math::IsNearZero(value.z)) value.z = 0.0f;
					if (ImGui::DragFloat3(name.c_str(), value,1,0,0,"%.3f deg")) {
						Rot value2(value);
						p.setter(obj, value2);
					}
				} else if (p.type.is<String>()) {
					str = p.get<String>(obj);
					if (ImGui::InputText(name.c_str(), &str)) {
						p.setter(obj, String(str));
					}
				} else if (p.type.is<Color>()) {
					auto value = p.get<Color>(obj);
					if (ImGui::ColorEdit4(name.c_str(), reinterpret_cast<f32*>(&value), ImGuiColorEditFlags_Float)) {
						p.setter(obj, value);
					}
				} else if (p.type.is<IntColor>()) {
					auto value = Color(p.get<IntColor>(obj));
					if (ImGui::ColorEdit4(name.c_str(), reinterpret_cast<f32*>(&value), ImGuiColorEditFlags_Uint8)) {
						p.setter(obj, value.toIntColor());
					}
				}

				if (!p.canWrite()) ImGui::EndDisabled();

				if (ImGui::IsItemHovered()) {
					if (auto desc = p.findTag("Description")) {
						name = *desc;
						ImGui::SetTooltip("%s",name.c_str());
					}
				}

			}

			for (auto& base : info->bases) {
				draw(obj, base);
			}
		}

	}

};

//***********************************************************
//! @file
//! @brief		ImGUi追加ウィジェット
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Library/imgui.h"
#include <Framework/Core/Core.h>
#include <Plugins/ImGui/ImGuiRAII.h>

namespace ImGui {
    inline namespace ob {

        using namespace ::ob;

        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::DrawFloatのVecラッパー
        //@―---------------------------------------------------------------------------
        //! @{
        inline bool DragFloat2(const char* label, ob::Vec2& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
            float values[] = { v.x,v.y };
            if (::ImGui::DragFloat2(label, values, v_speed, v_min, v_max, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                return true;
            }
            return false;
        }
        inline bool DragFloat3(const char* label, ob::Vec3& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
            float values[] = { v.x,v.y,v.z };
            if (::ImGui::DragFloat3(label, values, v_speed, v_min, v_max, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                v.z = values[2];
                return true;
            }
            return false;
        }
        inline bool DragFloat4(const char* label, ob::Vec4& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
            float values[] = { v.x,v.y,v.z,v.w };
            if (::ImGui::DragFloat4(label, values, v_speed, v_min, v_max, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                v.z = values[2];
                v.w = values[3];
                return true;
            }
            return false;
        }
        //! @}
        //! 

        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::DrawIntのVecラッパー
        //@―---------------------------------------------------------------------------
        //! @{
        inline bool DragInt2(const char* label, ob::IntVec2& v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) {
            int values[] = { v.x,v.y };
            if (::ImGui::DragInt2(label, values, v_speed, v_min, v_max, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                return true;
            }
            return false;
        }
        inline bool DragInt3(const char* label, ob::IntVec3& v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) {
            int values[] = { v.x,v.y,v.z };
            if (::ImGui::DragInt3(label, values, v_speed, v_min, v_max, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                v.z = values[2];
                return true;
            }
            return false;
        }
        inline bool DragInt4(const char* label, ob::IntVec4& v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) {
            int values[] = { v.x,v.y,v.z,v.w };
            if (::ImGui::DragInt4(label, values, v_speed, v_min, v_max, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                v.z = values[2];
                v.w = values[3];
                return true;
            }
            return false;
        }
        //! @}


        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::SliderFloatのVecラッパー
        //@―---------------------------------------------------------------------------
        //! @{
        inline bool SliderFloat2(const char* label, ob::Vec2& v, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
            float values[] = { v.x,v.y };
            if (::ImGui::SliderFloat2(label, values, v_min, v_max, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                return true;
            }
            return false;
        }
        inline bool SliderFloat3(const char* label, ob::Vec3& v, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
            float values[] = { v.x,v.y,v.z };
            if (::ImGui::SliderFloat3(label, values, v_min, v_max, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                v.z = values[2];
                return true;
            }
            return false;
        }
        inline bool SliderFloat4(const char* label, ob::Vec4& v, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
            float values[] = { v.x,v.y,v.z,v.w };
            if (::ImGui::SliderFloat4(label, values, v_min, v_max, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                v.z = values[2];
                v.w = values[3];
                return true;
            }
            return false;
        }
        //! @}


        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::SliderIntのVecラッパー
        //@―---------------------------------------------------------------------------
        //! @{
        inline bool SliderInt2(const char* label, ob::IntVec2& v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) {
            int values[] = { v.x,v.y };
            if (::ImGui::SliderInt2(label, values, v_min, v_max, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                return true;
            }
            return false;
        }
        inline bool SliderInt3(const char* label, ob::IntVec3& v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) {
            int values[] = { v.x,v.y,v.z };
            if (::ImGui::SliderInt3(label, values, v_min, v_max, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                v.z = values[2];
                return true;
            }
            return false;
        }
        inline bool SliderInt4(const char* label, ob::IntVec4& v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) {
            int values[] = { v.x,v.y,v.z,v.w };
            if (::ImGui::SliderInt4(label, values, v_min, v_max, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                v.z = values[2];
                v.w = values[3];
                return true;
            }
            return false;
        }
        //! @}


        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::InputFloatのVecラッパー
        //@―---------------------------------------------------------------------------
        //! @{
        inline bool InputFloat2(const char* label, ob::Vec2& v, const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
            float values[] = { v.x,v.y };
            if (::ImGui::InputFloat2(label, values, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                return true;
            }
            return false;
        }
        inline bool InputFloat3(const char* label, ob::Vec3& v, const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
            float values[] = { v.x,v.y,v.z };
            if (::ImGui::InputFloat3(label, values, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                v.z = values[2];
                return true;
            }
            return false;
        }
        inline bool InputFloat4(const char* label, ob::Vec4& v, const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
            float values[] = { v.x,v.y,v.z,v.w };
            if (::ImGui::InputFloat4(label, values, format, flags)) {
                v.x = values[0];
                v.y = values[1];
                v.z = values[2];
                v.w = values[3];
                return true;
            }
            return false;
        }
        //! @}


        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::InputIntのVecラッパー
        //@―---------------------------------------------------------------------------
        //! @{
        inline bool InputInt2(const char* label, ob::IntVec2& v, ImGuiInputTextFlags flags = 0) {
            int values[] = { v.x,v.y };
            if (::ImGui::InputInt2(label, values, flags)) {
                v.x = values[0];
                v.y = values[1];
                return true;
            }
            return false;
        }
        inline bool InputInt3(const char* label, ob::IntVec3& v, ImGuiInputTextFlags flags = 0) {
            int values[] = { v.x,v.y,v.z };
            if (::ImGui::InputInt3(label, values, flags)) {
                v.x = values[0];
                v.y = values[1];
                v.z = values[2];
                return true;
            }
            return false;
        }
        inline bool InputInt4(const char* label, ob::IntVec4& v, ImGuiInputTextFlags flags = 0) {
            int values[] = { v.x,v.y,v.z,v.w };
            if (::ImGui::InputInt4(label, values, flags)) {
                v.x = values[0];
                v.y = values[1];
                v.z = values[2];
                v.w = values[3];
                return true;
            }
            return false;
        }
        //! @}


        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::InputIntのVecラッパー
        //@―---------------------------------------------------------------------------
        //! @{
        inline bool ColorEdit3(const char* label, ob::Color& color, ImGuiColorEditFlags flags = 0) {
            float values[] = { color.r,color.g,color.b };
            if (::ImGui::ColorEdit3(label, values, flags)) {
                color.r = values[0];
                color.g = values[1];
                color.b = values[2];
                return true;
            }
            return false;
        }
        inline bool ColorEdit4(const char* label, ob::Color& color, ImGuiColorEditFlags flags = 0) {
            float values[] = { color.r,color.g,color.b,color.a };
            if (::ImGui::ColorEdit4(label, values, flags)) {
                color.r = values[0];
                color.g = values[1];
                color.b = values[2];
                color.a = values[3];
                return true;
            }
            return false;
        }
        inline bool ColorPicker3(const char* label, ob::Color& color, ImGuiColorEditFlags flags = 0) {
            float values[] = { color.r,color.g,color.b };
            if (::ImGui::ColorPicker3(label, values, flags)) {
                color.r = values[0];
                color.g = values[1];
                color.b = values[2];
                return true;
            }
            return false;
        }
        inline bool ColorPicker4(const char* label, ob::Color& color, ImGuiColorEditFlags flags = 0) {
            float values[] = { color.r,color.g,color.b,color.a };
            if (::ImGui::ColorPicker4(label, values, flags)) {
                color.r = values[0];
                color.g = values[1];
                color.b = values[2];
                color.a = values[3];
                return true;
            }
            return false;
        }
        //! @}

        //@―---------------------------------------------------------------------------
        //! @brief  ボタン式ラジオボタン
        //@―---------------------------------------------------------------------------
        //! @{
        inline bool ColorRadioButton(const char* label, bool active, Color color, const ImVec2& size = ImVec2(0, 0)) {

            Color hovered = Color::Lerp(color, Color::White, 0.2);
            if (active == false) {
                color *= Color(0.2);
            }

            ScopedButtonColor sbc(color);
            ScopedButtonHoveredColor sbhc(hovered);

            return ImGui::Button(label, size);
        }

    }
}
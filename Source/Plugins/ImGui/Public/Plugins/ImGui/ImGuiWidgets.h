//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Library/imgui.h"
#include <Framework/Core/Core.h>

namespace ImGui {

    //@―---------------------------------------------------------------------------
    //! @brief  ImGui::DrawFloatのVecラッパー
    //@―---------------------------------------------------------------------------
    //! @{
    inline bool DragFloat2(const char* label, ob::Vec2& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        float values[] = { v.x,v.y };
        if (DragFloat2(label, values, v_speed, v_min, v_max, format, flags)) {
            v.x = values[0];
            v.y = values[1];
            return true;
        }
        return false;
    }
    inline bool DragFloat3(const char* label, ob::Vec3& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        float values[] = { v.x,v.y,v.z };
        if (DragFloat3(label, values, v_speed, v_min, v_max, format, flags)) {
            v.x = values[0];
            v.y = values[1];
            v.z = values[2];
            return true;
        }
        return false;
    }
    inline bool DragFloat4(const char* label, ob::Vec4& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        float values[] = { v.x,v.y,v.z,v.w };
        if (DragFloat4(label, values, v_speed, v_min, v_max, format, flags)) {
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
        if (DragInt2(label, values, v_speed, v_min, v_max, format, flags)) {
            v.x = values[0];
            v.y = values[1];
            return true;
        }
        return false;
    }
    inline bool DragInt3(const char* label, ob::IntVec3& v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        int values[] = { v.x,v.y,v.z };
        if (DragInt3(label, values, v_speed, v_min, v_max, format, flags)) {
            v.x = values[0];
            v.y = values[1];
            v.z = values[2];
            return true;
        }
        return false;
    }
    inline bool DragInt4(const char* label, ob::IntVec4& v, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        int values[] = { v.x,v.y,v.z,v.w };
        if (DragInt4(label, values, v_speed, v_min, v_max, format, flags)) {
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
        if (SliderFloat2(label, values, v_min, v_max, format, flags)) {
            v.x = values[0];
            v.y = values[1];
            return true;
        }
        return false;
    }
    inline bool SliderFloat3(const char* label, ob::Vec3& v, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        float values[] = { v.x,v.y,v.z };
        if (SliderFloat3(label, values, v_min, v_max, format, flags)) {
            v.x = values[0];
            v.y = values[1];
            v.z = values[2];
            return true;
        }
        return false;
    }
    inline bool SliderFloat4(const char* label, ob::Vec4& v, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0) {
        float values[] = { v.x,v.y,v.z,v.w };
        if (SliderFloat4(label, values, v_min, v_max, format, flags)) {
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
        if (SliderInt2(label, values, v_min, v_max, format, flags)) {
            v.x = values[0];
            v.y = values[1];
            return true;
        }
        return false;
    }
    inline bool SliderInt3(const char* label, ob::IntVec3& v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        int values[] = { v.x,v.y,v.z };
        if (SliderInt3(label, values, v_min, v_max, format, flags)) {
            v.x = values[0];
            v.y = values[1];
            v.z = values[2];
            return true;
        }
        return false;
    }
    inline bool SliderInt4(const char* label, ob::IntVec4& v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0) {
        int values[] = { v.x,v.y,v.z,v.w };
        if (SliderInt4(label, values, v_min, v_max, format, flags)) {
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
        if (InputFloat2(label, values, format, flags)) {
            v.x = values[0];
            v.y = values[1];
            return true;
        }
        return false;
    }
    inline bool InputFloat3(const char* label, ob::Vec3& v, const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
        float values[] = { v.x,v.y,v.z };
        if (InputFloat3(label, values, format, flags)) {
            v.x = values[0];
            v.y = values[1];
            v.z = values[2];
            return true;
        }
        return false;
    }
    inline bool InputFloat4(const char* label, ob::Vec4& v, const char* format = "%.3f", ImGuiInputTextFlags flags = 0) {
        float values[] = { v.x,v.y,v.z,v.w };
        if (InputFloat4(label, values, format, flags)) {
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
        if (InputInt2(label, values, flags)) {
            v.x = values[0];
            v.y = values[1];
            return true;
        }
        return false;
    }
    inline bool InputInt3(const char* label, ob::IntVec3& v, ImGuiInputTextFlags flags = 0) {
        int values[] = { v.x,v.y,v.z };
        if (InputInt3(label, values, flags)) {
            v.x = values[0];
            v.y = values[1];
            v.z = values[2];
            return true;
        }
        return false;
    }
    inline bool InputInt4(const char* label, ob::IntVec4& v, ImGuiInputTextFlags flags = 0) {
        int values[] = { v.x,v.y,v.z,v.w };
        if (InputInt4(label, values, flags)) {
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
        if (ColorEdit3(label, values, flags)) {
            color.r = values[0];
            color.g = values[1];
            color.b = values[2];
            return true;
        }
        return false;
    }
    inline bool ColorEdit4(const char* label, ob::Color& color, ImGuiColorEditFlags flags = 0) {
        float values[] = { color.r,color.g,color.b,color.a };
        if (ColorEdit4(label, values, flags)) {
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
        if (ColorPicker3(label, values, flags)) {
            color.r = values[0];
            color.g = values[1];
            color.b = values[2];
            return true;
        }
        return false;
    }
    inline bool ColorPicker4(const char* label, ob::Color& color, ImGuiColorEditFlags flags = 0) {
        float values[] = { color.r,color.g,color.b,color.a };
        if (ColorPicker4(label, values, flags)) {
            color.r = values[0];
            color.g = values[1];
            color.b = values[2];
            color.a = values[3];
            return true;
        }
        return false;
    }
    //! @}

}// namespcae ob
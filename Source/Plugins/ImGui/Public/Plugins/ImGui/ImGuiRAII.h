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
    //! @brief  ImGui::PushFontのRAII対応
    //@―---------------------------------------------------------------------------
    class ScopedFont {
    public:
        ScopedFont(ImFont* font) {
            PushFont(font);
        }
        ~ScopedFont() {
            PopFont();
        }
    };

    //@―---------------------------------------------------------------------------
    //! @brief  ImGui::PushStyleColorのRAII対応
    //@―---------------------------------------------------------------------------
    class ScopedStyleColor {
    public:
        ScopedStyleColor(ImGuiCol idx, const ob::Color& col) {
            ImVec4 imcol(col.r, col.g, col.b, col.a);
            PushStyleColor(idx, imcol);
        }
        virtual ~ScopedStyleColor() {
            PopStyleColor();
        }
    };
    class ScopedTextColor :public ScopedStyleColor {
    public:
        ScopedTextColor(const ob::Color& col) :ScopedStyleColor(ImGuiCol_Text, col) {}
    };
    class ScopedButtonColor :public ScopedStyleColor {
    public:
        ScopedButtonColor(const ob::Color& col) :ScopedStyleColor(ImGuiCol_Button, col) {}
    };

    //@―---------------------------------------------------------------------------
    //! @brief  ImGui::PushStyleVarのRAII対応
    //@―---------------------------------------------------------------------------
    class ScopedStyleVar {
    public:
        ScopedStyleVar(ImGuiStyleVar idx, float val) {
            PushStyleVar(idx, val);
        }
        virtual ~ScopedStyleVar() {
            PopStyleColor();
        }
    };

    //@―---------------------------------------------------------------------------
    //! @brief  ImGui::PushStyleVarのRAII対応(Vec2)
    //@―---------------------------------------------------------------------------
    class ScopedStyleVec2Var {
    public:
        ScopedStyleVec2Var(ImGuiStyleVar idx, const ob::Vec2& val) {
            ImVec2 v(val.x, val.y);
            PushStyleVar(idx, v);
        }
        virtual ~ScopedStyleVec2Var() {
            PopStyleColor();
        }
    };

    //@―---------------------------------------------------------------------------
    //! @brief  ImGui::PushItemWidthのRAII対応
    //@―---------------------------------------------------------------------------
    class ScopedItemWidth {
    public:
        ScopedItemWidth(float item_width) {
            PushItemWidth(item_width);
        }
        ~ScopedItemWidth() {
            PopItemWidth();
        }
    };

    //@―---------------------------------------------------------------------------
    //! @brief  ImGui::IndentのRAII対応
    //@―---------------------------------------------------------------------------
    class ScopedIndent {
    public:
        ScopedIndent(float indent_w = 0.0f) 
            : m_indent(indent_w)
        {
            Indent(indent_w);
        }
        ~ScopedIndent() {
            Unindent(m_indent);
        }
    private:
        float m_indent;
    };

    //@―---------------------------------------------------------------------------
    //! @brief  ImGui::BeginGroupのRAII対応
    //@―---------------------------------------------------------------------------
    class ScopedGroup {
    public:
        ScopedGroup() {
            BeginGroup();
        }
        ~ScopedGroup() {
            EndGroup();
        }
    };

    //@―---------------------------------------------------------------------------
    //! @brief  ImGui::PushIDのRAII対応
    //@―---------------------------------------------------------------------------
    class ScopedID{
    public:
        ScopedID(const char* str_id) {
            PushID(str_id);
        }
        ScopedID(const char* str_id_begin, const char* str_id_end) {
            PushID(str_id_begin, str_id_end);
        }
        ScopedID(const void* ptr_id) {
            PushID(ptr_id);
        }
        ScopedID(int int_id) {
            PushID(int_id);
        }
        ~ScopedID() {
            PopFont();
        }
    };

}// namespcae ob
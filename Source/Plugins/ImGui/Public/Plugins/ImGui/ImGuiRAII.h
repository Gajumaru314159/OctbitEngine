//***********************************************************
//! @file
//! @brief		ImGui RAII対応
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Library/imgui.h"
#include <Framework/Core/Core.h>

namespace ImGui {
    inline namespace ob {

        using namespace ::ob;

        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::PushFontのRAII対応
        //@―---------------------------------------------------------------------------
        class ScopedFont {
        public:
            ScopedFont(ImFont* font) {
                ::ImGui::PushFont(font);
            }
            ~ScopedFont() {
                ::ImGui::PopFont();
            }
        };

        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::PushStyleColorのRAII対応
        //@―---------------------------------------------------------------------------
        class ScopedStyleColor {
        public:
            ScopedStyleColor(ImGuiCol idx, const ob::Color& col) {
                ImVec4 imcol(col.r, col.g, col.b, col.a);
                ::ImGui::PushStyleColor(idx, imcol);
            }
            virtual ~ScopedStyleColor() {
                ::ImGui::PopStyleColor();
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
        class ScopedButtonHoveredColor :public ScopedStyleColor {
        public:
            ScopedButtonHoveredColor(const ob::Color& col) :ScopedStyleColor(ImGuiCol_ButtonHovered, col) {}
        };
        class ScopedButtonActiveColor :public ScopedStyleColor {
        public:
            ScopedButtonActiveColor(const ob::Color& col) :ScopedStyleColor(ImGuiCol_ButtonActive, col) {}
        };

        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::PushStyleVarのRAII対応
        //@―---------------------------------------------------------------------------
        class ScopedStyleVar {
        public:
            ScopedStyleVar(ImGuiStyleVar idx, float val) {
                ::ImGui::PushStyleVar(idx, val);
            }
            virtual ~ScopedStyleVar() {
                ::ImGui::PopStyleVar();
            }
        };

        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::PushStyleVarのRAII対応(Vec2)
        //@―---------------------------------------------------------------------------
        class ScopedStyleVec2Var {
        public:
            ScopedStyleVec2Var(ImGuiStyleVar idx, const ob::Vec2& val) {
                ImVec2 v(val.x, val.y);
                ::ImGui::PushStyleVar(idx, v);
            }
            virtual ~ScopedStyleVec2Var() {
                ::ImGui::PopStyleVar();
            }
        };

        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::PushItemWidthのRAII対応
        //@―---------------------------------------------------------------------------
        class ScopedItemWidth {
        public:
            ScopedItemWidth(float item_width) {
                ::ImGui::PushItemWidth(item_width);
            }
            ~ScopedItemWidth() {
                ::ImGui::PopItemWidth();
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
                ::ImGui::Indent(indent_w);
            }
            ~ScopedIndent() {
                ::ImGui::Unindent(m_indent);
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
                ::ImGui::BeginGroup();
            }
            ~ScopedGroup() {
                ::ImGui::EndGroup();
            }
        };

        //@―---------------------------------------------------------------------------
        //! @brief  ImGui::PushIDのRAII対応
        //@―---------------------------------------------------------------------------
        class ScopedID {
        public:
            ScopedID(const char* str_id) {
                ::ImGui::PushID(str_id);
            }
            ScopedID(const char* str_id_begin, const char* str_id_end) {
                ::ImGui::PushID(str_id_begin, str_id_end);
            }
            ScopedID(const void* ptr_id) {
                ::ImGui::PushID(ptr_id);
            }
            ScopedID(int int_id) {
                ::ImGui::PushID(int_id);
            }
            ~ScopedID() {
                ::ImGui::PopID();
            }
        };

    }
}
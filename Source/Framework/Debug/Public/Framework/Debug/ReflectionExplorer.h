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

	class ReflectionExplorer {
	public:
		ReflectionExplorer();
		void draw();
	private:
		void draw(const TypeInfo&);
		void drawDetail(const TypeInfo&);
	private:
		String m_buffer;
		Type m_selected;
	};

}
//***********************************************************
//! @file
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
		Inspector() = default;
		void draw(Any& obj, Type type) const;
	};

}
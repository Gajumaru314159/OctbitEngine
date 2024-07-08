//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeBuilder.h>

//@―---------------------------------------------------------------------------
//! @brief		Enum型情報登録
//@―---------------------------------------------------------------------------
#define OB_REGISTER_RTTI(TypeName )\
namespace type_info_builder::TypeName {\
	void Register();\
	struct Registrator : public ReflectionFunction {\
		Registrator() : ReflectionFunction(Register) { PushReflectionFunction(this); }\
	} g_registrator;\
}
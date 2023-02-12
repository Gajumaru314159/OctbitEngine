//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Core.h>
#include <Framework/Core/Reflection/TypeId.h>
#include <Framework/Engine/Reflection/TypeInfo.h>

//@―---------------------------------------------------------------------------
//! @brief		Enum型情報登録
//@―---------------------------------------------------------------------------
#define OB_REGISTER_RTTI(type)\
type_info_builder::type::Link();
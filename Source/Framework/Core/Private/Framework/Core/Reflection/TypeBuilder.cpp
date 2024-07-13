//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeBuilder.h>
#include <Framework/Core/Reflection/TypeInfo.h>
#include <Framework/Core/Core.h>

namespace type_info_builder {

	ReflectionFunction* g_reflectionFunction = nullptr;

	ReflectionFunction* GetReflectionFunction() {
		return g_reflectionFunction;
	}
	void PushReflectionFunction(ReflectionFunction* func) {
		func->next = g_reflectionFunction;
		g_reflectionFunction = func;
	}

}


namespace ob::core::internal {

	//===============================================================
	// TagBuilder
	//===============================================================

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	TagBuilder::TagBuilder(TagInfo& info)
		: m_info(&info)
	{
	}

	//@―---------------------------------------------------------------------------
	//! @brief		タグ追加
	//@―---------------------------------------------------------------------------
	void TagBuilder::tag(StringView key, StringView value) {
		m_info->tags.emplace(key, value);
	}


	//===============================================================
	// EnumBuilder
	//===============================================================

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	EnumBuilder::EnumBuilder(TypeInfo& info)
		: TagBuilder(info)
		, m_info(info)
	{
	}


	//===============================================================
	// ClassBuilder
	//===============================================================

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	ClassBuilder::ClassBuilder(TypeInfo& info)
		: TagBuilder(info)
		, m_info(info)
	{
	}

}
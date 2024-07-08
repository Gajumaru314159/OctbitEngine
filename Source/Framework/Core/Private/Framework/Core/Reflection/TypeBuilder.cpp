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

	//@―---------------------------------------------------------------------------
	//! @brief		要素追加
	//! @details	追加した順番にインデックスが割り振られます。インデックスは0ベースです。
	//@―---------------------------------------------------------------------------
	TagBuilder EnumBuilder::elementImpl(StringView name, s64 value){
		auto& element = m_info.enumElements.emplace_back();
		element.name = name;
		element.index = m_info.enumElements.size() - 1;
		element.value = value;
		return element;
	};


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

	//@―---------------------------------------------------------------------------
	//! @brief		タグ追加
	//@―---------------------------------------------------------------------------
	void ClassBuilder::baseImpl(Type type) {
		m_info.bases.emplace(type);
	}


}
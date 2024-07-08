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
	// ElementBuilder
	//===============================================================

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	ElementBuilder::ElementBuilder(ElementInfo& info)
		: m_info(info)
	{
		m_info.tags.clear();
	}

	//@―---------------------------------------------------------------------------
	//! @brief		タグ追加
	//@―---------------------------------------------------------------------------
	void ElementBuilder::tag(StringView key, StringView value) {
		m_info.tags.emplace(key, value);
	}


	//===============================================================
	// EnumBuilder
	//===============================================================

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	EnumBuilder::EnumBuilder(EnumInfo& info)
		: m_info(info)
	{
		m_info.elements.clear();
	}

	//@―---------------------------------------------------------------------------
	//! @brief		タグ追加
	//@―---------------------------------------------------------------------------
	void EnumBuilder::tag(StringView key, StringView value) {
		m_info.tags.emplace(key, value);
	}

	//@―---------------------------------------------------------------------------
	//! @brief		要素追加
	//! @details	追加した順番にインデックスが割り振られます。インデックスは0ベースです。
	//@―---------------------------------------------------------------------------
	ElementInfo& EnumBuilder::elementImpl(StringView name, s64 value){
		auto& element = m_info.elements.emplace_back();
		element.name = name;
		element.index = m_info.elements.size() - 1;
		element.value = value;
		return element;
	};


	//===============================================================
	// PropertyBuilder
	//===============================================================

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	PropertyBuilder::PropertyBuilder(PropertyInfo* info)
		: m_info(info)
	{
		if(m_info)m_info->tags.clear();
	}

	//@―---------------------------------------------------------------------------
	//! @brief		タグ追加
	//@―---------------------------------------------------------------------------
	void PropertyBuilder::tag(StringView key, StringView value) {
		if (m_info)m_info->tags.emplace(key, value);
	}


	//===============================================================
	// FunctionBuilder
	//===============================================================

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	FunctionBuilder::FunctionBuilder(FunctionInfo& info)
		: m_info(info)
	{
		m_info.tags.clear();
	}

	//@―---------------------------------------------------------------------------
	//! @brief		タグ追加
	//@―---------------------------------------------------------------------------
	void FunctionBuilder::tag(StringView key, StringView value) {
		m_info.tags.emplace(key, value);
	}


	//===============================================================
	// ClassBuilder
	//===============================================================

	//@―---------------------------------------------------------------------------
	//! @brief		コンストラクタ
	//@―---------------------------------------------------------------------------
	ClassBuilder::ClassBuilder(ClassInfo& info)
		: m_info(info)
	{
		m_info.tags.clear();
	}

	//@―---------------------------------------------------------------------------
	//! @brief		タグ追加
	//@―---------------------------------------------------------------------------
	void ClassBuilder::tag(StringView key, StringView value ) {
		m_info.tags.emplace(key, value);
	}

	void ClassBuilder::baseImpl(TypeId typeId) {
		m_info.bases.emplace(typeId);
	}

	//@―---------------------------------------------------------------------------
	//! @brief		基底クラス登録
	//! @details	SFINAEで不正な型をコンパイルエラーにするために実装はClassBuilderTemplateでしています。
	//@―---------------------------------------------------------------------------
	void ClassBuilder::constructorImpl() {

	}

	//@―---------------------------------------------------------------------------
	//! @brief		基底クラス登録
	//! @details	SFINAEで不正な型をコンパイルエラーにするために実装はClassBuilderTemplateでしています。
	//@―---------------------------------------------------------------------------
	FunctionBuilder ClassBuilder::functionImpl(StringView name) {
		FunctionInfo f;
		f.name = name;
		m_info.functions.emplace(name,f);
		return FunctionBuilder(f);
	}

	void ClassBuilder::convert(s32 fromVersion, s32 toVersion, StringView oldName, StringView newName) {
	}


	void ClassBuilder::version(s32 version/*,VersionConverter converter = nullptr*/) {
	};

}
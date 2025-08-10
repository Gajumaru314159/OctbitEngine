//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/TypeBuilder.h>
#include <Framework/Core/Reflection/TypeInfo.h>
#include <Framework/Core/Core.h>

namespace ob::core {

	ReflectionFunction* g_reflectionFunction = nullptr;

	ReflectionFunction::ReflectionFunction(func_type func) :func(func) {
		PushReflectionFunction(this);
	}

	ReflectionFunction* GetReflectionFunction() {
		return g_reflectionFunction;
	}
	void PushReflectionFunction(ReflectionFunction* func) {
		func->next = g_reflectionFunction;
		g_reflectionFunction = func;
	}


	//===============================================================
	// TagBuilder
	//===============================================================

	//! @brief		コンストラクタ
	TagBuilder::TagBuilder(TagInfo& info)
		: m_tagInfo(info)
	{
	}

	//! @brief		タグ追加
	TagBuilder& TagBuilder::tag(StringView key, StringView value) {
		m_tagInfo.tags.emplace(key, value);
		return *this;
	}

	//! @brief		説明をDescriptionタグとして追加
	TagBuilder& TagBuilder::desc(StringView value) {
		tag("Description", value);
		return *this;
	}

	//===============================================================
	// EnumBuilder
	//===============================================================

	//! @brief		コンストラクタ
	EnumBuilder::EnumBuilder(TypeInfo& info)
		: TagBuilder(info)
		, m_info(info)
	{
	}


	//===============================================================
	// ClassBuilder
	//===============================================================

	//! @brief		コンストラクタ
	ClassBuilder::ClassBuilder(TypeInfo& info)
		: TagBuilder(info)
		, m_info(info)
	{
	}


	StringView ClassBuilder::_GetDefaultArgumentName(size_t index) {
		StringView names[] = {
			"arg0",
			"arg1",
			"arg2",
			"arg3",
			"arg4",
			"arg5",
			"arg6",
			"arg7",
			"arg8",
			"arg9",
		};
		if (std::size(names) <= index) return "argX";
		return names[index];
	}

}
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>

namespace ob::core {

	bool IsCastable(Type from, Type to);
	
	template<class To, class From>
	bool IsCastable(From&&) {
		return IsCastable(Type::Get<From>(), Type::Get<To>());	
	}

	template<class From, class To>
	bool IsCastable() {
		return IsCastable(Type::Get<From>(), Type::Get<To>());
	}

	template<class To,class From>
	To* DynamicCast(From&& from) {
		if (!IsCastable<To>(from)) return nullptr;
		return reinterpret_cast<To*>(&from);
	}

}
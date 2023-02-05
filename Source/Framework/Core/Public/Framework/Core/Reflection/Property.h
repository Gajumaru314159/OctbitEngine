//***********************************************************
//! @file
//! @brief		プロパティ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {

	class TypeInfo;
	class Object;

	struct UserTypeObject {

	};


	using PropertyVariant = Variant<
		bool,
		s8,
		s16,
		s32,
		s64,
		u8,
		u16,
		u32,
		u64,
		f32,
		f64,
		UserTypeObject
	>;


	class Accessor {

	};


	//@―---------------------------------------------------------------------------
	//! @brief  プロパティ
	//@―---------------------------------------------------------------------------
	class Property {
	public:
		virtual void setValue(const PropertyVariant& value) = 0;
		virtual PropertyVariant getValue() = 0;
	protected:

	private:
		Object* m_owner;
		PropertyInfo* minfo;
	};

}// namespcae ob

//***********************************************************
//! @file
//! @brief		プロパティ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Core.h>
#include <Framework/Engine/Reflection/TypeInfo.h>

namespace ob::engine::rtti {

	//using PropertyVariant = Variant<
	//	bool,
	//	s8,
	//	s16,
	//	s32,
	//	s64,
	//	u8,
	//	u16,
	//	u32,
	//	u64,
	//	f32,
	//	f64,
	//	UserTypeObject
	//>;

	//@―---------------------------------------------------------------------------
	//! @brief  プロパティ
	//@―---------------------------------------------------------------------------
	class PropertyObject {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		PropertyObject(void* target, const PropertyInfo&);

		//@―---------------------------------------------------------------------------
		//! @brief  有効な値か
		//@―---------------------------------------------------------------------------
		operator bool()const noexcept{ return m_target != nullptr; }

		//@―---------------------------------------------------------------------------
		//! @brief  値設定
		//@―---------------------------------------------------------------------------
		template<class T>
		void setValue(T&& value) {
			T copy = value;
			setValueImpl(TypeId::Get<T>(), &copy);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  値取得
		//@―---------------------------------------------------------------------------
		template<class T>
		Optional<T> getValue() {
			if (auto ret = getValueImpl(TypeId::Get<T>())) {
				return *reinterpret_cast<const T*>(ret);
			}
			return std::nullopt;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  値取得
		//@―---------------------------------------------------------------------------
		template<class T>
		const T* getValuePtr() {
			if (auto ret = getValueImpl(TypeId::Get<T>())) {
				return reinterpret_cast<const T*>(ret);
			}
			return std::nullopt;
		}

		//@―---------------------------------------------------------------------------
		//! @brief  プロパティ型チェック
		//@―---------------------------------------------------------------------------
		template<class T>
		bool is() const noexcept {
			return m_info.type == TypeId::Get<T>();
		}

		//@―---------------------------------------------------------------------------
		//! @brief  プロパティ情報取得
		//@―---------------------------------------------------------------------------
		const PropertyInfo& getPropertyInfo()const noexcept {
			return m_info;
		}

	protected:

		void setValueImpl(TypeId typeId, void* value);
		const void* getValueImpl(TypeId typeId)const;

	private:

		void* m_target;
		const PropertyInfo& m_info;
	};


	class FunctionObject {
	public:

		template<class R,typename... Args>
		R call(Args&& ...args) {

		}

	private:
	};

}// namespcae ob

﻿//***********************************************************
//! @file
//! @brief		EntityHandle
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Forward.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief		Entity識別子
	//! @details	EntityHandle は Entity を一位に識別するために
	//@―---------------------------------------------------------------------------
	class EntityHandle {
		friend class std::hash<class EntityHandle>;
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		デフォルトコンストラクタ
		//@―---------------------------------------------------------------------------
		EntityHandle() = default;

		//@―---------------------------------------------------------------------------
		//! @brief		UUIDから構築
		//@―---------------------------------------------------------------------------
		EntityHandle(const UUID& uuid);

		//@―---------------------------------------------------------------------------
		//! @brief		有効か
		//! @details	EntityHandle が割り当てられたEntityが存在する場合はtrueを返します。
		//@―---------------------------------------------------------------------------
		operator bool()const { return !get(); }

		//@―---------------------------------------------------------------------------
		//! @brief		等価演算子
		//@―---------------------------------------------------------------------------
		constexpr bool operator==(const EntityHandle& rhs)const { return m_uuid == rhs.m_uuid; }

		//@―---------------------------------------------------------------------------
		//! @brief		否等価演算子
		//@―---------------------------------------------------------------------------
		constexpr bool operator!=(const EntityHandle& rhs)const { return m_uuid != rhs.m_uuid; }

		//@―---------------------------------------------------------------------------
		//! @brief		
		//! @details	EntityHandle が割り当てられたEntityが存在する場合はtrueを返します。
		//@―---------------------------------------------------------------------------
		bool empty()const { return !get(); }

		//@―---------------------------------------------------------------------------
		//! @brief		
		//@―---------------------------------------------------------------------------
		void clear();

		//@―---------------------------------------------------------------------------
		//! @brief		この EntityHandle が割り当てられたEntityのポインタを取得します。
		//@―---------------------------------------------------------------------------
		Entity* get()const;

	private:
		UUID m_uuid;
	};

	//@―---------------------------------------------------------------------------
	//! @brief			EntityHandleのリスト
	//@―---------------------------------------------------------------------------
	using EntityHandleList = List<EntityHandle>;

}// namespcae ob



namespace std {

	template<>
	struct hash<ob::engine::EntityHandle> {
		constexpr size_t operator()(const ob::engine::EntityHandle& value)const {
			return std::hash<ob::core::UUID>()(value.m_uuid);
		}
	};

}
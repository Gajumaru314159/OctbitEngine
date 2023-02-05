//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::engine {

	class Entity;

	//@―---------------------------------------------------------------------------
	//! @brief  コンポーネント
	//@―---------------------------------------------------------------------------
	class Component {
	public:

		~Component();

		OB_RTTI();

		Entity& getEntity()const { return *m_entity; }

	protected:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		Component();

		virtual void startup() {};
		virtual void activate() {};
		virtual void deactivate() {};

		
	private:

		class Entity* m_entity;

	};

}// namespcae ob

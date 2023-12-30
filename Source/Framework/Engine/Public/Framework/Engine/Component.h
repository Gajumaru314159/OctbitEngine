﻿//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeId.h>
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/PropertyNotifier.h>

namespace ob::engine {

	//@―---------------------------------------------------------------------------
	//! @brief  コンポーネント定義マクロ
	//@―---------------------------------------------------------------------------
#define OB_COMPONENT(type)\
	public:\
		OB_RTTI();\
		static type* Create();\
		virtual TypeId getComponentTypeId() const{return TypeId::Get<type>();}

	//@―---------------------------------------------------------------------------
	//! @brief  コンポーネント実装マクロ
	//@―---------------------------------------------------------------------------
#define OB_COMPONENT_CPP(type,gen)\
	type* type::Create() { return new gen; }



	//@―---------------------------------------------------------------------------
	//! @brief  コンポーネントのリスト
	//@―---------------------------------------------------------------------------
	using ComponentList = List<UPtr<Component>>;

	//@―---------------------------------------------------------------------------
	//! @brief  コンポーネント
	//@―---------------------------------------------------------------------------
	class Component:public PropertyNotifier {
	public:

		OB_RTTI();

		~Component();

		//@―---------------------------------------------------------------------------
		//! @brief		コンポーネントのインターフェイス取得
		//! @details	コンポ―ネントのインターフェイスと実装を分離する場合に使用する
		//@―---------------------------------------------------------------------------
		virtual TypeId getComponentTypeId()const { return getTypeId(); };

		//@―---------------------------------------------------------------------------
		//! @brief		所属するEntityを取得
		//@―---------------------------------------------------------------------------
		Entity& getEntity()const;

		//@―---------------------------------------------------------------------------
		//! @brief		所属シーン取得
		//@―---------------------------------------------------------------------------
		Scene* getScene()const;

		//@―---------------------------------------------------------------------------
		//! @brief		所属ワールド取得
		//@―---------------------------------------------------------------------------
		World* getWorld()const;

	protected:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//! @details	構築時は Entity の参照を持っていないため Entity や他の Compoent
		//!				にアクセスできません。依存がある初期設定は startup() を使用してください。
		//@―---------------------------------------------------------------------------
		Component();

		//@―---------------------------------------------------------------------------
		//! @brief		初期化
		//! @details	この関数は Entity ツリーの構築後に一度だけ呼び出されます。
		//!				初期化に Entity や他の Componentを利用する場合に使用します。
		//!				非同期で実行されます。
		//!				依存コンポ―ネントは初期化前です。
		//@―---------------------------------------------------------------------------
		virtual void initialize() {};

		//@―---------------------------------------------------------------------------
		//! @brief		起動
		//! @details	シーンの構築完了後に一度だけ呼び出されます。
		//@―---------------------------------------------------------------------------
		virtual void startup() {};

		//@―---------------------------------------------------------------------------
		//! @brief		アクティブ化
		//! @details	所有している Entity がアクティブ化されるときに呼び出されます。
		//!				依存する Component がある場合、依存 Componet がアクティブ化されたあとに呼び出されます。
		//!				アクティブ状態の Entity に追加された場合は startup() 後に呼び出されます。
		//@―---------------------------------------------------------------------------
		virtual void activate() {};

		//@―---------------------------------------------------------------------------
		//! @brief		非アクティブ化
		//! @details	所有している Entity が非アクティブ化されるときに呼び出されます。
		//!				非アクティブ化の順序はアクティブ化の逆順であるため、
		//!				コンポーネントは依存コンポーネントより前に非アクティブ化されます。
		//@―---------------------------------------------------------------------------
		virtual void deactivate() {};
		
	private:

		// 構築直後にEntityから設定する用
		friend class Entity;

		Entity* m_entity = nullptr;

	};

}

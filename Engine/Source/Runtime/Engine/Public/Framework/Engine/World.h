//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/Scene.h>

namespace ob::engine {

	class SubSystemContainer {
	public:
		//! @brief		サブシステムを取得
		template<class T>
		T* find()const { return nullptr; }

		//! @brief		同じ型のサブシステムを尊くすることはできません
		template<class T>
		bool add(T&&) {
			//m_systems.try_emplace()
			return false;
		}


	private:
		HashMap<Type, void*> m_systems;
	};

	//! @brief  ワールド
	//! @see	https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/Engine/UWorld/
	class World {
	public:

		static World* Create(StringView name) {
			return new World(name);
		}

	public:

		//! @brief		コンストラクタ
		~World();

		//! @brief		ルートシーンを取得
		Scene& getRootScene()const;


		//! @brief		サブシステムを取得
		template<class T>
		T* findSubSystem()const { return nullptr; }

		//! @brief		同じ型のサブシステムを尊くすることはできません
		template<class T>
		bool addSubSystem(T&&){
			return false;
		}

	private:

		World(StringView name);

		void createRootScene();

	private:

		String m_name;

		Scene* m_scene = nullptr;

	};

}

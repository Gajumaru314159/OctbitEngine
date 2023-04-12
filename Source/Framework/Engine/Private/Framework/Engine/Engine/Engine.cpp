//***********************************************************
//! @file
//! @brief		エンジン
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Core.h>
#include <Framework/Engine/Engine.h>

#include <Framework/Engine/Component/ComponentFactory.h>
#include <Framework/Engine/Entity/EntityManager.h>

// DI完了後削除
namespace ob {
	namespace platform {
		class WindowManager;
	}
	namespace input {
		class InputModule;
	}
	namespace graphics {
		class GraphicModule;
	}
	namespace rhi {
		class SystemResource;
	}
}

namespace ob::engine {

	struct EngineDependency {
		EngineDependency(
			EntityManager&,
			platform::WindowManager&,
			input::InputModule&,
			graphics::GraphicModule&,
			rhi::SystemResource&
		) {}
	};


	//@―---------------------------------------------------------------------------
	//! @brief  コンストラクタ
	//@―---------------------------------------------------------------------------
	Engine::Engine(ServiceInjector& injector)
	{
		injector.bind<EntityManager>();
		injector.bind<ComponentFactory>();
		injector.bind<EngineDependency>();

		if (injector.create<EngineDependency>(m_container) == nullptr) {
			LOG_FATAL("依存関係の構築に失敗");
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  デストラクタ
	//@―---------------------------------------------------------------------------
	Engine::~Engine() {
		LOG_INFO("[Shutdown OctbitEngine]");
	}

	//@―---------------------------------------------------------------------------
	//! @brief				起動
	//@―---------------------------------------------------------------------------
	void Engine::startup() {
	}


	template<class T>
	void update_debug(T* instance) {
		if (instance) {
			instance->update();
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  更新
	//@―---------------------------------------------------------------------------
	bool Engine::update() {

		update_debug(get<EntityManager>());

		return true;
	}

}
//***********************************************************
//! @file
//! @brief		エンジン
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Core.h>
#include <Framework/Engine/Engine.h>

#include <Framework/Engine/Component/ComponentFactory.h>
#include <Framework/Engine/Entity/EntityManager.h>

#include <Framework/Input/InputManager.h>
#include <Framework/RHI/RHI.h>
#include <Framework/Graphics/Graphics.h>

// DI完了後削除
namespace ob {
	namespace platform {
		class WindowManager;
	}
	namespace input {
		class InputModule;
	}
	namespace graphics {
		class Graphics;
		class MaterialManager;
	}
	namespace rhi {
		class SystemResource;
	}
}

namespace ob::engine {

	struct EngineDependency {
		EngineDependency(
			EntityManager&,
			platform::WindowManager*,
			input::InputModule*,
			graphics::Graphics*,
			graphics::MaterialManager*,
			rhi::SystemResource*
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

	template<class T>
	void update_debug() {
		if (auto instance = T::Get()) {
			instance->update();
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief  更新
	//@―---------------------------------------------------------------------------
	bool Engine::update() {

		update_debug<EntityManager>();

		update_debug<input::InputModule>();
		update_debug<rhi::RHI>();
		update_debug<graphics::Graphics>();

		return true;
	}

}
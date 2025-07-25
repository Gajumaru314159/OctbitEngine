//***********************************************************
//! @file
//! @brief		エンジン
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Core.h>
#include <Framework/Engine/Engine.h>

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

	//! @brief  コンストラクタ
	Engine::Engine(ServiceInjector& injector)
	{
		injector.bind<EntityManager>();

		injector.createAll(m_container);
	}

	//! @brief  デストラクタ
	Engine::~Engine() {
		LOG_INFO("[Shutdown OctbitEngine]");
	}

	template<class T>
	void update_debug() {
		if (auto instance = T::Get()) {
			instance->update();
		}
	}

	//! @brief  更新
	bool Engine::update() {

		update_debug<EntityManager>();

		update_debug<input::InputModule>();
		update_debug<rhi::Device>();

		return true;
	}

}
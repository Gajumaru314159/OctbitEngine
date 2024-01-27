//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Platform/Window.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/RHI/All.h>
#include <Plugins/DirectX12RHI/System.h>
#include <Windows.h>

#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Render/RenderScene.h>
using namespace ob;

TEST(Graphis, Create) {

	using namespace ob::rhi;
	using namespace ob::graphics;

	ServiceInjector injector;
	ServiceContainer container;
	rhi::dx12::Register(injector);
	rhi::Register(injector);
	graphics::Register(injector);

	injector.create<Graphics>(container);

	RenderSceneDesc desc;
	desc.name = TC("Test");

	auto scene = RenderScene::Create(desc);
	auto view = scene->createView();

	for (s32 i = 0; i < 100; ++i) {

		if (auto graphics = container.get<Graphics>()) {
			graphics->update();
		}

	}

}
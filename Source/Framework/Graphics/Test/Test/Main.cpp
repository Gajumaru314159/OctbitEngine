#include <Windows.h>

#include <Framework/Core/Utility/DI.h>

#include <Framework/RHI/All.h>
#include <Framework/Input/All.h>
#include <Framework/Platform/Window.h>

#include <Framework/Graphics/Material.h>
#include <Framework/Graphics/Mesh.h>

#include <Framework/Platform/System.h>
#include <Framework/Input/System.h>
#include <Framework/Input/InputManager.h>
#include <Framework/Graphics/System.h>
#include <Framework/Graphics/Graphics.h>
#include <Plugins/DirectX12RHI/System.h>

#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderPipeline.h>
#include <Framework/Graphics/Render/RenderPipelineDesc.h>
#include <Framework/Graphics/Feature/ForwardRenderFeature.h>

//-----------------------------------------------------------------
using namespace ob;

void OctbitInit(ServiceInjector& injector) {

	input::
		Register(injector);
	rhi::dx12::
		Register(injector);
	graphics::
		Register(injector);

}

int OctbitMain() {

	using namespace ob::rhi;
	using namespace ob::graphics;

	// ウィンドウ生成
	platform::WindowDesc windowDesc;
	windowDesc.title = TC("Graphic Test");
	platform::Window window(windowDesc);



	auto renderScene = graphics::RenderScene::Create(TC("TestScene"));
	{
		graphics::RenderPipelineDesc rdesc;
		rdesc.name = TC("TestPipeline");
		//rdesc.features.push_back(std::move(std::make_unique<graphics::ForwardRenderFeature>()));

		auto pip = renderScene->createRenderPipeline<graphics::RenderPipeline>(std::move(rdesc));
	}

	return 0;
}
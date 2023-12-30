#include <Windows.h>

#include <Framework/Core/Utility/DI.h>

#include <Framework/RHI/All.h>
#include <Framework/Input/All.h>
#include <Framework/Platform/Window.h>

#include <Framework/Graphics/System.h>
#include <Framework/Graphics/RPI.h>
#include <Plugins/DirectX12RHI/System.h>

#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Feature/MeshRenderFeature.h>
#include <Framework/Graphics/Feature/ImGuiRenderFeature.h>
#include <Framework/Graphics/Render/RenderPipeline.h>

//-----------------------------------------------------------------
using namespace ob;

void OctbitInit(ServiceInjector& injector) {

	rhi::dx12::
		Register(injector);
	graphics::
		Register(injector);

}

class UniversalRenderPipeline :public graphics::RenderPipeline {

};

ob::graphics::RenderFeatureInjector ReflectRenderFeatureInjector() {
	ob::graphics::RenderFeatureInjector injector;

	//TypeInfo::Visit(TypeId::Get<ob::graphics::RenderFeature>(),
	//	[injector](const TypeInfo& info) {
	//		injector.add(info.typeId, [](ob::graphics::RenderScene& scene) {return typeInfo.create<RenderFeature*>(scene); });
	//	}
	//);

	return std::move(injector);
}

int OctbitMain() {

	using namespace ob::rhi;
	using namespace ob::graphics;

	// ウィンドウ生成
	platform::WindowDesc windowDesc;
	windowDesc.title = TC("Graphic Test");
	platform::Window window(windowDesc);


	// TODO リフレクション登録対応
	RenderSceneDesc sdesc;
	sdesc.name = TC("Default");
	sdesc.features.add<MeshRenderFeature>();
	sdesc.features.add<ImGuiRenderFeature>();
	// sdesc.pipelines.add<ForwardRenderingPipeline>();

	auto scene = RenderScene::Create(sdesc);

	// TODO アセットから生成対応
	auto view = scene->createView<UniversalRenderPipeline>();

	if (auto feature = scene->findFeature<MeshRenderFeature>()) {
		auto id = feature->createMesh();
	}

	while (true) {

		// Job

		auto& rpi = *RPI::Get();

		rpi.update();

		rpi.wait();
		rpi.execute();
		
	}

	return 0;
}
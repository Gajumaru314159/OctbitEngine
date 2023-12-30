#include <Windows.h>

#include <Framework/Core/Utility/DI.h>

#include <Framework/RHI/All.h>
#include <Framework/Input/All.h>
#include <Framework/Platform/Window.h>

#include <Framework/Graphics/System.h>
#include <Framework/Graphics/Graphics.h>
#include <Plugins/DirectX12RHI/System.h>

#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderFeatureSet.h>
#include <Framework/Graphics/Feature/MeshRenderFeature.h>
#include <Framework/Graphics/Feature/ImGuiRenderFeature.h>

//-----------------------------------------------------------------
using namespace ob;

void OctbitInit(ServiceInjector& injector) {

	rhi::dx12::
		Register(injector);
	graphics::
		Register(injector);

}

int OctbitMain() {
	
	Ref<Model> model;

	model = Model::Load("Assets/Cube.fbx");

	model.setMatrix(matrix);
	model.setBoneMatrix("l_arm",matrix);



	return 0;
}
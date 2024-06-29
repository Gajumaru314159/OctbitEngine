//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Platform/System.h>
#include <Framework/Platform/Window.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/RHI/All.h>
#include <Plugins/DirectX12RHI/System.h>
#include <Windows.h>

#include <Framework/Graphics/Graphics.h>
#include <Framework/Graphics/Render/RenderScene.h>
#include <Framework/Graphics/Render/RenderView.h>
#include <Framework/Graphics/Feature/ImGuiRenderFeature.h>

#include <Framework/Input/System.h>
#include <Framework/Input/InputManager.h>

#include <Framework/Debug/LogInfo.h>
#include <Framework/Debug/Profiler.h>

#include <Framework/Model/Model.h>
#include <Framework/Graphics/Material.h>

using namespace ob;
using namespace ob::graphics;

namespace ob::graphics {
	class MaterialManager;
}


class SampleRenderPipeline : public RenderPipeline {
public:
	SampleRenderPipeline(RenderScene& scene)
		: RenderPipeline(scene)
	{

	}

	//@―---------------------------------------------------------------------------
	//! @brief      描画
	//@―---------------------------------------------------------------------------
	void render(FG& fg, const Vector<Ref<RenderView>>& views) {

		// RenderFeature更新
		// 例：Shadow/GI

		for (auto& view : views) {

			FGTexture::Desc desc;
			FGTexture target{ view->getRenderTarget() };


			auto resource = fg.import("Target",desc,std::move(target));

			render<ImGuiStep>(view,fg, resource);
		}

	}

	template<class T,class... Args>
	void render(const Ref<RenderView>& view,Args&&... args) {
		if (auto step = view->findStep<T>()) {
			step->render(args...);
		} else {

		}
	}

};

TEST(Graphis, Create) {

	using namespace ob::rhi;
	using namespace ob::graphics;
	using namespace ob::platform;

	System::Setup();
	
	ServiceInjector injector;

	rhi::dx12::Register(injector);
	rhi::Register(injector);
	input::Register(injector);
	graphics::Register(injector);

	struct Dependency {
		Dependency(ob::graphics::Graphics&,ob::input::InputModule&,ob::graphics::MaterialManager&){}
	};
	injector.bind<Dependency>();

	ServiceContainer container;
	injector.create<Dependency>(container);


	ob::debug::Profiler profiler;
	ob::debug::LogInfo loginfo;

	// ウィンドウ生成
	platform::WindowDesc windowDesc;
	windowDesc.title = "Graphic Test";
	platform::Window window(windowDesc);

	Ref<Display> display = [&] {
		DisplayDesc desc;
		desc.name = "MainDisplay";
		desc.window = window;
		return Display::Create(desc);
	}();

	auto scene = [&]{
		RenderSceneDesc desc;
		desc.name = "Test";
		desc.features.add<ImGuiRenderFeature>();
		desc.pipelines.add<SampleRenderPipeline>();
		return RenderScene::Create(desc);
	}();

	auto view = [&] {
		RenderViewDesc desc;
		desc.name = "MainCamera";
		desc.size = { 1280,720 };
		desc.type = RenderViewType::Camera;
		return scene->createView(desc);
	}();
	view->setRenderTarget(display);

	ImGuiHandle handle;
	scene->findFeature<ImGuiRenderFeature>()->addTask(handle,
		[&] {
			profiler.update();
			loginfo.update();
		}
	);


	Ref<RootSignature> signature;
	{
		RootSignatureDesc desc(
			{
				RootParameter::Range(DescriptorRangeType::CBV,1,0),		// バッファ
				RootParameter::Range(DescriptorRangeType::SRV,1,0),		// テクスチャ
				//RootParameter::Range(DescriptorRangeType::Sampler,1,0),	// サンプラー
				RootParameter::Range(DescriptorRangeType::CBV,1,1),		// バッファ
				RootParameter::Range(DescriptorRangeType::SRV,1,1),		// テクスチャ
				//RootParameter::Range(DescriptorRangeType::Sampler,1,1),	// サンプラー
			},
			{
				StaticSamplerDesc(SamplerDesc(TextureFillter::Point),0),
			}
			);
		desc.name = "Common";
		signature = RootSignature::Create(desc);
		OB_ASSERT_EXPR(signature);
	}

	Ref<Shader> vs;
	Ref<Shader> ps;
	{
		String code = R"(

SamplerState g_mainSampler:register(s0);

// Global
Texture2D s_skyTex:register(t0);
cbuffer Param : register(b0) {
  //float    s_scalars[4];
  float4   s_colors[1];
  float4x4 s_matrices[1];
};

// Local
Texture2D g_mainTex:register(t1);
cbuffer Param : register(b1) { 
  //float    g_scalars[4];
  float4   g_colors[1];
  float4x4 g_matrices[1];
};

// IN / OUT
struct VsIn {
  float4 pos	:POSITION;
  float4 normal	:NORMAL;
  float2 uv		:TEXCOORD;
};
struct PsIn {
  float4 pos	:SV_POSITION;
  float4 normal	:NORMAL;
  float2 uv		:TEXCOORD;
};
struct PsOut {
  float4 color0	:SV_TARGET0;
  float4 color1	:SV_TARGET1;
};
// エントリ
PsIn VS_Main(VsIn i) {
    PsIn o;
    o.pos = mul(s_matrices[0],mul(g_matrices[0],float4(i.pos.xyz,1)));
    o.uv = i.uv;
    o.normal = i.normal;
    return o;
}
PsOut PS_Main(PsIn i){
    PsOut o;
    float4 color = g_mainTex.Sample(g_mainSampler,i.uv) * g_colors[0];
	color.rgb *= (dot(i.normal.xyz,s_colors[0].xyz)*0.25+0.25+0.5);
    o.color1 = color;o.color1.xyz=1-o.color1.xyz;
    o.color0 = color;
    return o;
}
			)";

		vs = Shader::CompileVS(code);
		ps = Shader::CompilePS(code);
		OB_ASSERT_EXPR(vs && ps);
	}

	auto material = [&]() {

		graphics::MaterialPass pass;
		pass.rasterizer.cullMode = CullMode::None;
		pass.depthStencil.depth.enable = true;
		pass.depthStencil.stencil.enable = false;
		pass.vs = vs;
		pass.ps = ps;

		// ShaderGroup
		pass.requiredLayout = {
			InputLayout(Semantic::Position,Type::Float,3,0),
			InputLayout(Semantic::Normal,Type::Float,3,0),
			InputLayout(Semantic::TexCoord,Type::Float,2,0),
		};
		// ShaderGroup
		pass.rootSignature = signature;

		graphics::MaterialDesc desc;
		desc.colorProperties = { "Color" };
		desc.matrixProperties = { "Matrix" };
		desc.textureProperties = { "Main" };
		desc.passes[Name("Opaque")] = pass;

		return graphics::Material::Create(desc);
	}();

	auto model = model::Model::Load("Asset/Model/Ukulele.obj");

	model->setMaterial("Ukulele", material);

	model->join(scene.get());
	
	//model->setMaterial("Ukulele", );


	for (s32 i = 0; i < 1000; ++i) {

		if (System::Update() == false)break;

		RHI::Get()->update();
		input::InputModule::Get()->update();

		display->update();

		if (auto graphics = container.get<Graphics>()) {
			graphics->update();
		}

	}

}
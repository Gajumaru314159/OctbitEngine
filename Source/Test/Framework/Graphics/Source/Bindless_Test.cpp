//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/DI.h>
#include <Framework/Graphics/All.h>
#include <Framework/RHI/All.h>
#include <Framework/Platform/System.h>
#include <Framework/Platform/Window.h>
#include <Plugins/DirectX12RHI/System.h>
#include <Framework/Graphics/Material/MaterialBlock.h>

namespace ob::rhi {
	class SystemResource;
}

TEST(Bindless, Handle) {
#pragma region
	
	using namespace ob;
	using namespace ob::rhi;
	using namespace ob::graphics;
	using namespace ob::platform;
	ob::core::Logger log;

	System::Setup();

	ServiceInjector injector;
	ServiceContainer container;
	{
		rhi::dx12::RegisterDirectX12RHIService(injector);
		graphics::RegisterGraphicsService(injector);
		{
			rhi::RHIConfig config;
			config.enablePIX = true;
			config.enableBindless = true;
			//config.enableDebugLayer = true;
			injector.bind(config);
		}

		struct Dependency {
			Dependency(ob::graphics::Graphics&, SystemResource&) {}
		};
		injector.bind<Dependency>();

		injector.create<Dependency>(container);
	}


	// ウィンドウ生成
	platform::WindowDesc windowDesc;
	windowDesc.title = "Bindless Handle";
	platform::Window window(windowDesc);
	window.show();

	Ref<Display> display = [&] {
		DisplayDesc desc;
		desc.name = "MainDisplay";
		desc.window = window;
		return Display::Create(desc);
		}();


	Ref<CommandList> commandList;
	{
		CommandListDesc desc;
		desc.name = "Main";
		desc.type = CommandListType::Graphic;
		commandList = CommandList::Create(desc);
	}


	Ref<RenderTexture> renderTexture;
	{
		RenderTextureDesc desc;
		desc.name = "RenderTexture";
		desc.size = display->getDesc().size;
		desc.format = TextureFormat::RGBA8;
		desc.clear.color = Color::Black;
		desc.display = display;
		renderTexture = RenderTexture::Create(desc);
	}

#pragma endregion

	struct Vertex {
		Vec2 pos;
		Vec2 uv;
	};

	Ref<Buffer> vertexBuffer;
	Ref<Buffer> indexBuffer;
	{
		Vertex vertices[] = {
			{Vec2(-1,+1),Vec2(0,0)},
			{Vec2(+1,+1),Vec2(1,0)},
			{Vec2(-1,-1),Vec2(0,1)},
			{Vec2(+1,-1),Vec2(1,1)},
		};
		u16 indices[] = { 0,1,2,3,2,1 };

		vertexBuffer = Buffer::Create(BufferDesc::Vertex<Vertex>(4));
		indexBuffer = Buffer::Create(BufferDesc::Index<u16>(6));

		vertexBuffer->updateDirect(sizeof(vertices), vertices);
		indexBuffer->updateDirect(sizeof(indices), indices);
	}

	Ref<RootSignature> signature;
	{
		RootSignatureDesc desc(
			{
				RootParameter::Constants(32,0)
			}
		);
		desc.name = "MaterialBlock";
		desc.flags.set(RootSignatureFlag::EnableBindless, true);
		signature = RootSignature::Create(desc);
		OB_ASSERT_EXPR(signature);
	}

	// 事前セットアップここまで
	Ref<Shader> vs;
	Ref<Shader> ps;
	{
		ShaderCompileDesc desc;
		if (auto file = File::ReadAllText("Assets/Shader/BindlessTest.hlsl")) {
			desc.code = file.value();
		}
		desc.directories = { "Assets/Shader" };
		desc.stage = ShaderStage::Vertex;
		vs = Shader::Compile(desc);
		desc.stage = ShaderStage::Pixel;
		ps = Shader::Compile(desc);
		OB_ASSERT_EXPR(vs && ps);
	}

	Ref<PipelineState> pipeline;
	{
		PipelineStateDesc desc;
		desc.name = "MaterialBlock";
		desc.colors = { TextureFormat::RGBA8 };

		desc.rootSignature = signature;
		desc.vs = vs;
		desc.ps = ps;
		desc.vertexLayout.attributes = {
			VertexAttribute(Semantic::Position,offsetof(Vertex,pos),ElementType::Float,2),
			VertexAttribute(Semantic::TexCoord,offsetof(Vertex,uv),ElementType::Float,2),
		};
		desc.rasterizer.cullMode = CullMode::None;
		pipeline = PipelineState::Create(desc);
		OB_ASSERT_EXPR(pipeline);
	}


	MaterialBlockDesc desc;
	desc.name = "TestBlock";
	desc.vectors = { "color" };
	desc.textures = { "albedo" };

	MaterialBlock block(desc);
	block.setVector("color", Color::Cyan);
	block.setTexture("albedo", Texture::Check(), Sampler::Default());

	auto start = DateTime::Now();

	f32 endTime = 5.0f;

	while (true) {

		if (System::Update() == false) break;

		auto time = TimeSpan(start, DateTime::Now()).totalSecondsF();

		if (endTime < time)break;

		commandList->begin();

		Viewport viewport;
		viewport.right = renderTexture->width();
		viewport.bottom = renderTexture->height();
		commandList->setViewport(&viewport, 1);

		commandList->setRenderTarget(renderTexture);
		commandList->clearColors();

		commandList->setPipelineState(pipeline);
		block.record(commandList, 0);

		commandList->setVertexBuffer(vertexBuffer);
		commandList->setIndexBuffer(indexBuffer);

		DrawIndexedParam param;
		param.indexCount = 6;
		param.startIndex = 0;
		param.startVertex = 0;
		commandList->drawIndexed(param);

		commandList->applyDisplay(display, renderTexture);

		commandList->end();
		commandList->flush();

		if (auto graphics = container.get<Graphics>()) {
			graphics->update();
		}

		RHI::Get()->update();
		display->update();

		Thread::Sleep(33);
	}

}
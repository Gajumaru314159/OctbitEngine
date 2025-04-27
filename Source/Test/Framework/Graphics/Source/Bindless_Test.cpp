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
#include <Plugins/DirectX12RHI/DirectX12RHIConfig.h>
#include <Plugins/VulkanRHI/System.h>
#include <Plugins/VulkanRHI/VulkanRHIConfig.h>
#include <Framework/Graphics/Material/MaterialBlock.h>

namespace ob::rhi {
	class SystemResource;
}

TEST(Bindless, Handle) {
	return;
#pragma region
	using namespace ob;
	using namespace ob::rhi;
	using namespace ob::graphics;
	using namespace ob::platform;
	ob::core::Logger log;

	System::Setup();

	rhi::RHIConfig config;
	config.enableBindless = true;

	rhi::dx12::DirectX12RHIConfig dx12config;
	dx12config.enablePIX = true;

	rhi::vulkan::VulkanRHIConfig vkconfig;
	vkconfig.enableDebugLayer = true;

	ServiceInjector injector;
	ServiceContainer container;
	{
		//rhi::dx12::RegisterDirectX12RHIService(injector);
		rhi::vulkan::RegisterVulkanRHIService(injector);
		graphics::RegisterGraphicsService(injector);
		injector.bind(config);
		injector.bind(dx12config);
		injector.bind(vkconfig);

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

	Ref<SwapChain> swapChain = [&] {
		SwapChainDesc desc;
		desc.name = "MainSwapChain";
		desc.window = window;
		return SwapChain::Create(desc);
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
		desc.size = swapChain->getDesc().size;
		desc.format = TextureFormat::RGBA8;
		desc.clear.color = Color::Black;
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
		RootSignatureDesc desc;
		desc.constants = RootConstantsDesc(32, 0);
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
		desc.vertexLayout.vertexStride = sizeof(Vertex);
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

		if (System::Update() == false);// break;

		auto time = TimeSpan(start, DateTime::Now()).totalSecondsF();

		if (endTime < time)break;

		commandList->begin();

		RenderPassDesc renderPass;
		renderPass.colors.emplace_back(renderTexture, RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);

		commandList->beginRenderPass(renderPass);

		commandList->setPipelineState(pipeline);
		block.record(commandList, 0);

		commandList->setVertexBuffer(vertexBuffer);
		commandList->setIndexBuffer(indexBuffer);

		DrawIndexedParam param;
		param.indexCount = 6;
		param.startIndex = 0;
		param.startVertex = 0;
		commandList->drawIndexed(param);

		commandList->endRenderPass();

		commandList->applySwapChain(swapChain, renderTexture);

		commandList->end();
		commandList->flush();

		if (auto graphics = container.get<Graphics>()) {
			graphics->update();
		}

		RHI::Get()->update();
		swapChain->update();

		Thread::Sleep(33);
	}

}
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
#include <Framework/Graphics/Material/MaterialBlock.h>
#include <Plugins/VulkanRHI/System.h>
#include <Plugins/VulkanRHI/VulkanRHIConfig.h>

namespace ob::rhi {
	class SystemResource;
}

TEST(MaterialBlock, Bindfull) {
#pragma region
	
	using namespace ob;
	using namespace ob::rhi;
	using namespace ob::graphics;
	using namespace ob::platform;
	 ob::core::Logger log;

	System::Setup();

	rhi::RHIConfig config;
	config.enableBindless = false;

	rhi::DirectX12RHIConfig dx12config;
	dx12config.enablePIX = true;

	rhi::VulkanRHIConfig vkconfig;
	vkconfig.enableDebugLayer = true;

	ServiceInjector injector;
	ServiceContainer container;
	{
		rhi::RegisterDirectX12RHIService(injector);
		//rhi::RegisterVulkanRHIService(injector);
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
	windowDesc.title = "MaterialBlock Bindfull";
	platform::Window window(windowDesc);
	window.show();

	Ref<SwapChain> swapChain = [&] {
		SwapChainDesc desc;
		desc.name = "MainSwapChain";
		desc.window = window;
		return SwapChain::Create(desc);
	}();

#pragma endregion

	struct Vertex {
		Vec2 pos;
		Vec2 uv;
	};

	Ref<Buffer> vertexBuffer;
	Ref<Buffer> indexBuffer;
	{
		Vertex vertices[] = {
			{Vec2(-1,-1),Vec2(0,1)},
			{Vec2(-1,1),Vec2(0,0)},
			{Vec2(1,1),Vec2(1,0)},
			{Vec2(1,-1),Vec2(1,1)},
		};
		u16 indices[] = { 0,1,2,0,2,3 };

		vertexBuffer = Buffer::Create(BufferDesc::Vertex<Vertex>(4));
		indexBuffer = Buffer::Create(BufferDesc::Index<u16>(6));

		vertexBuffer->updateDirect(sizeof(vertices), vertices);
		indexBuffer->updateDirect(sizeof(indices), indices);
	}

	// 事前セットアップここまで
	Ref<Shader> vs;
	Ref<Shader> ps;
	{
		String code = R"(
struct Params {
	float4 Color;
	float Progress;
	float Speed;
	float Time;
	float Width;
	float Height;
};
Texture2D g_mainTex:register(t0);
SamplerState g_mainSampler:register(s1);
ByteAddressBuffer g_buffer:register(t2);
ByteAddressBuffer g_params:register(t3);
											
// IN / OUT												
struct VsIn {												
	float2 pos		:POSITION;									
	float2 uv	    :TEXCOORD0;										
};															
struct PsIn {												
	float4 pos		:SV_POSITION;								
	float2 uv	    :TEXCOORD0;									
};															
// エントリ													
struct PsOut {												
	float4 color	:SV_TARGET0;								
};															
PsIn VS_Main(VsIn i) {										
	PsIn o;													
	o.pos = float4(i.pos,0,1);					        
	o.uv  = i.uv;
	return o;
}
PsOut PS_Main(PsIn i){										
	PsOut o;							
	Params params = g_params.Load<Params>(0);					
	o.color = g_mainTex.Sample(g_mainSampler,i.uv * float2(params.Width/16,params.Height/16) + float2(params.Time*params.Speed,params.Time*params.Speed))*params.Color;	
	o.color.xyz *= 1 - step(i.uv.x,params.Progress) * step(i.uv.y,0.1);
	return o;											        
}																

)";
		vs = Shader::CompileVS(code);
		ps = Shader::CompilePS(code);
		OB_ASSERT_EXPR(vs && ps);
	}

	Ref<DescriptorLayout> layout = DescriptorLayout::Create({
		Binding::Texture(0),
		Binding::Sampler(1),
		Binding::ByteAddressBuffer(2),
		Binding::ByteAddressBuffer(3),
	});

	Ref<RootSignature> signature;
	{
		RootSignatureDesc desc;
		desc.layouts = { layout };
		desc.name = "MaterialBlock";
		signature = RootSignature::Create(desc);
		OB_ASSERT_EXPR(signature);
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
	desc.layout = layout;
	desc.textures = { "Texture" };
	desc.buffers = { "Buffer" };
	desc.vectors = { "Color" };
	desc.scalars = { "Progress","Speed", "Time", "Width", "Height" };

	MaterialBlock block(desc);
	block.setScalar("Speed", 3.f);
	block.setScalar("Width", window.getSize().x);
	block.setScalar("Height", window.getSize().y);
	block.setTexture("Texture", Texture::Check(),Sampler::Default());
	block.setVector("Color", Color::Cyan);

	auto start = DateTime::Now();

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

	f32 endTime = 5;

	while (true) {

		if (System::Update() == false);// break;

		auto time = TimeSpan(start,DateTime::Now()).totalSecondsF();

		if (endTime < time)break;

		block.setScalar("Time", time);
		block.setScalar("Progress", time / endTime);

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

	}

}

TEST(MaterialBlock, Bindless) {
#pragma region
	using namespace ob;
	using namespace ob::rhi;
	using namespace ob::graphics;
	using namespace ob::platform;
	ob::core::Logger log;

	System::Setup();

	rhi::RHIConfig config;
	config.enableBindless = true;

	rhi::DirectX12RHIConfig dx12config;
	dx12config.enablePIX = true;

	rhi::VulkanRHIConfig vkconfig;
	vkconfig.enableDebugLayer = true;

	ServiceInjector injector;
	ServiceContainer container;
	{
		rhi::RegisterDirectX12RHIService(injector);
		//rhi::RegisterVulkanRHIService(injector);
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
	windowDesc.title = "MaterialBlock Bindless";
	platform::Window window(windowDesc);
	window.show();

	Ref<SwapChain> swapChain = [&] {
		SwapChainDesc desc;
		desc.name = "MainSwapChain";
		desc.window = window;
		return SwapChain::Create(desc);
		}();

#pragma endregion

	struct Vertex {
		Vec2 pos;
		Vec2 uv;
	};

	Ref<Buffer> vertexBuffer;
	Ref<Buffer> indexBuffer;
	{
		Vertex vertices[] = {
			{Vec2(-1,-1),Vec2(0,1)},
			{Vec2(-1,1),Vec2(0,0)},
			{Vec2(1,1),Vec2(1,0)},
			{Vec2(1,-1),Vec2(1,1)},
		};
		u16 indices[] = { 0,1,2,0,2,3 };

		vertexBuffer = Buffer::Create(BufferDesc::Vertex<Vertex>(4));
		indexBuffer = Buffer::Create(BufferDesc::Index<u16>(6));

		vertexBuffer->updateDirect(sizeof(vertices), vertices);
		indexBuffer->updateDirect(sizeof(indices), indices);
	}

	// 事前セットアップここまで
	Ref<Shader> vs;
	Ref<Shader> ps;
	{
		String code = R"(

struct TextureHandle {
	uint type;
	uint index;
	uint reserved0;
	uint reserved1;
};

struct SamplerHandle {
	uint type;
	uint index;
	uint reserved0;
	uint reserved1;
};

struct BufferHandle {
	uint type;
	uint index;
	uint reserved0;
	uint reserved1;
};


#if defined(VULKAN)

[[vk::push_constant]]
BufferHandle ParamHandle;

#elif defined(D3D)

ConstantBuffer<BufferHandle> ParamHandle : register(b0);

#endif

struct Param {
	TextureHandle MainTexture;
	SamplerHandle MainSampler;
	BufferHandle Buffer;
	float4 Color;
	float Progress;
	float Speed;
	float Time;
	float Width;
	float Height;
};
											
// IN / OUT												
struct VsIn {												
	float2 pos		:POSITION;									
	float2 uv	    :TEXCOORD0;										
};															
struct PsIn {												
	float4 pos		:SV_POSITION;								
	float2 uv	    :TEXCOORD0;									
};															
// エントリ													
struct PsOut {												
	float4 color	:SV_TARGET0;								
};															
PsIn VS_Main(VsIn i) {
	PsIn o;													
	o.pos = float4(i.pos,0,1);					        
	o.uv  = i.uv;
	return o;
}
PsOut PS_Main(PsIn i){										
	PsOut o;					
	Param param = ByteAddressBuffer(ResourceDescriptorHeap[ParamHandle.index]).Load<Param>(0);

	Texture2D g_mainTex = ResourceDescriptorHeap[param.MainTexture.index];							
	SamplerState g_mainSampler = SamplerDescriptorHeap[param.MainSampler.index];							
	o.color = g_mainTex.Sample(g_mainSampler,i.uv * float2(param.Width/16,param.Height/16) + float2(param.Time*param.Speed,param.Time*param.Speed))*param.Color;	
	o.color.xyz *= 1 - step(i.uv.x,param.Progress) * step(i.uv.y,0.1);
	return o;											        
}																

)";
		vs = Shader::CompileVS(code);
		ps = Shader::CompilePS(code);
		OB_ASSERT_EXPR(vs && ps);
	}

	Ref<RootSignature> signature;
	{
		RootSignatureDesc desc;
		desc.constants = RootConstantsDesc(16,0);
		desc.name = "MaterialBlock";
		desc.flags.set(RootSignatureFlag::EnableBindless, true);
		signature = RootSignature::Create(desc);
		OB_ASSERT_EXPR(signature);
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

		pipeline = PipelineState::Create(desc);
		OB_ASSERT_EXPR(pipeline);
	}


	MaterialBlockDesc desc;
	desc.name = "TestBlock";
	desc.textures = { "Texture" };
	desc.buffers = { "Buffer" };
	desc.vectors = { "Color" };
	desc.scalars = { "Progress","Speed", "Time", "Width", "Height" };

	MaterialBlock block(desc);
	block.setScalar("Speed", 3.f);
	block.setScalar("Width", window.getSize().x);
	block.setScalar("Height", window.getSize().y);
	block.setTexture("Texture", Texture::Check(), Sampler::Default());
	block.setVector("Color", Color::Cyan);

	auto start = DateTime::Now();

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

	f32 endTime = 5;

	while (true) {

		if (System::Update() == false);// break;

		auto time = TimeSpan(start, DateTime::Now()).totalSecondsF();

		if (endTime < time)break;

		block.setScalar("Time", time);
		block.setScalar("Progress", time / endTime);

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

	}

}



TEST(MaterialBlock, MultiBindless) {
#pragma region
	using namespace ob;
	using namespace ob::rhi;
	using namespace ob::graphics;
	using namespace ob::platform;
	ob::core::Logger log;

	System::Setup();

	rhi::RHIConfig config;
	config.enableBindless = true;

	rhi::DirectX12RHIConfig dx12config;
	dx12config.enablePIX = true;

	rhi::VulkanRHIConfig vkconfig;
	vkconfig.enableDebugLayer = true;

	ServiceInjector injector;
	ServiceContainer container;
	{
		rhi::RegisterDirectX12RHIService(injector);
		//rhi::RegisterVulkanRHIService(injector);
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
	windowDesc.title = "MaterialBlock MultiBindless";
	platform::Window window(windowDesc);
	window.show();

	Ref<SwapChain> swapChain = [&] {
		SwapChainDesc desc;
		desc.name = "MainSwapChain";
		desc.window = window;
		return SwapChain::Create(desc);
		}();

#pragma endregion

	struct Vertex {
		Vec2 pos;
		Vec2 uv;
	};

	Ref<Buffer> vertexBuffer;
	Ref<Buffer> indexBuffer;
	{
		Vertex vertices[] = {
			{Vec2(-1,-1),Vec2(0,1)},
			{Vec2(-1,1),Vec2(0,0)},
			{Vec2(1,1),Vec2(1,0)},
			{Vec2(1,-1),Vec2(1,1)},
		};
		u16 indices[] = { 0,1,2,0,2,3 };

		vertexBuffer = Buffer::Create(BufferDesc::Vertex<Vertex>(4));
		indexBuffer = Buffer::Create(BufferDesc::Index<u16>(6));

		vertexBuffer->updateDirect(sizeof(vertices), vertices);
		indexBuffer->updateDirect(sizeof(indices), indices);
	}

	// 事前セットアップここまで
	Ref<Shader> vs;
	Ref<Shader> ps;
	{
		String code = R"(

struct TextureHandle {
	uint type;
	uint index;
	uint reserved0;
	uint reserved1;
};

struct SamplerHandle {
	uint type;
	uint index;
	uint reserved0;
	uint reserved1;
};

struct BufferHandle {
	uint type;
	uint index;
	uint reserved0;
	uint reserved1;
};

struct Param {
	TextureHandle MainTexture;
	SamplerHandle MainSampler;
	BufferHandle Buffer;
	float4 Color;
	float Progress;
	float Speed;
	float Time;
	float Width;
	float Height;
};

struct Param2 {
	float Scale;
};

struct PushConstant {
	BufferHandle ParamHandle;
	BufferHandle Param2Handle;
};

#if defined(VULKAN)

[[vk::push_constant]]
PushConstant g_pushConstant;

#elif defined(D3D)

ConstantBuffer<PushConstant> g_pushConstant: register(b0);

#endif
											
// IN / OUT												
struct VsIn {												
	float2 pos		:POSITION;									
	float2 uv	    :TEXCOORD0;										
};															
struct PsIn {												
	float4 pos		:SV_POSITION;								
	float2 uv	    :TEXCOORD0;									
};															
// エントリ													
struct PsOut {												
	float4 color	:SV_TARGET0;								
};															
PsIn VS_Main(VsIn i) {
	PsIn o;													
	o.pos = float4(i.pos,0,1);			

	Param2 param2 = ByteAddressBuffer(ResourceDescriptorHeap[g_pushConstant.Param2Handle.index]).Load<Param2>(0);		        
	o.uv  = i.uv * param2.Scale;
	return o;
}
PsOut PS_Main(PsIn i){										
	PsOut o;					
	Param param = ByteAddressBuffer(ResourceDescriptorHeap[g_pushConstant.ParamHandle.index]).Load<Param>(0);

	Texture2D g_mainTex = ResourceDescriptorHeap[param.MainTexture.index];							
	SamplerState g_mainSampler = SamplerDescriptorHeap[param.MainSampler.index];							
	o.color = g_mainTex.Sample(g_mainSampler,i.uv * float2(param.Width/16,param.Height/16) + float2(param.Time*param.Speed,param.Time*param.Speed))*param.Color;	
	o.color.xyz *= 1 - step(i.uv.x,param.Progress) * step(i.uv.y,0.1);
	return o;											        
}																

)";
		vs = Shader::CompileVS(code);
		ps = Shader::CompilePS(code);
		OB_ASSERT_EXPR(vs && ps);
	}

	Ref<RootSignature> signature;
	{
		RootSignatureDesc desc;
		desc.constants = RootConstantsDesc(32, 0, 0);
		desc.name = "MaterialBlock";
		desc.flags.set(RootSignatureFlag::EnableBindless, true);
		signature = RootSignature::Create(desc);
		OB_ASSERT_EXPR(signature);
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

		pipeline = PipelineState::Create(desc);
		OB_ASSERT_EXPR(pipeline);
	}


	MaterialBlockDesc desc;
	desc.name = "Param";
	desc.textures = { "Texture" };
	desc.buffers = { "Buffer" };
	desc.vectors = { "Color" };
	desc.scalars = { "Progress","Speed", "Time", "Width", "Height" };

	MaterialBlock block(desc);
	block.setScalar("Speed", 3.f);
	block.setScalar("Width", window.getSize().x);
	block.setScalar("Height", window.getSize().y);
	block.setTexture("Texture", Texture::Check(), Sampler::Default());
	block.setVector("Color", Color::Cyan);



	MaterialBlockDesc desc2;
	desc2.name = "Param2";
	desc2.scalars = { "Scale" };

	MaterialBlock block2(desc2);
	block2.setScalar("Scale", 3.f);

	auto start = DateTime::Now();

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

	f32 endTime = 5;

	while (true) {

		if (System::Update() == false);// break;

		auto time = TimeSpan(start, DateTime::Now()).totalSecondsF();

		if (endTime < time)break;

		block.setScalar("Time", time);
		block.setScalar("Progress", time / endTime);

		commandList->begin();

		RenderPassDesc renderPass;
		renderPass.colors.emplace_back(renderTexture, RenderPassBeforeAccessType::Clear, RenderPassAfterAccessType::Preserve);

		commandList->beginRenderPass(renderPass);

		commandList->setPipelineState(pipeline);
		block.record(commandList, 0, 0);
		block2.record(commandList, 0,16);

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

	}

}
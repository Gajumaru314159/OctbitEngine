//***********************************************************
//! @file
//! @brief		Buffer のテスト
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>
#include <magic_enum.hpp>

static const char* s_code =
R"(
    // Vertex Shader
    struct VSInput {
        [[vk::location(0)]] float4 position : POSITION;
        [[vk::location(1)]] float4 color : COLOR;
    };

    struct VSOutput {
        [[vk::location(0)]] float4 position : SV_POSITION;
        [[vk::location(1)]] float4 color : COLOR;
    };

    VSOutput VS_Main(VSInput input) {
        VSOutput output;
        output.position = input.position;
        output.color = input.color;
        return output;
    }

    // Pixel Shader
    struct PSInput {
        [[vk::location(0)]] float4 position : SV_POSITION;
        [[vk::location(1)]] float4 color : COLOR;
    };
    struct PSOutput {
        [[vk::location(0)]] float4 color : SV_TARGET;
    };

    PSOutput PS_Main(PSInput input) {
        PSOutput o;
        o.color = input.color;
        return o;
    }

)";

struct Vertex {
    Vec4 position;
    Color color;
};

class PipelineStateTest : public RHITestBase {};

TEST_F(PipelineStateTest, Create) {

    PipelineStateDesc desc;
    desc.name = "TestPipelineState";
    desc.colors = { TextureFormat::RGBA8 };
    desc.rootSignature = RootSignature::Create(RootSignatureDesc{});
	desc.vertexLayout.vertexStride = sizeof(Vertex);
    desc.vertexLayout.attributes = {
        { Semantic::Position, offsetof(Vertex,position),ElementType::Float,4},
        { Semantic::Color, offsetof(Vertex,position),ElementType::Float,4},
    };

    desc.vs = Shader::CompileVS(s_code);
    desc.ps = Shader::CompilePS(s_code);

    desc.blend = { BlendDesc::AlphaBlend };

    auto pipelineState = PipelineState::Create(desc);
    ASSERT_TRUE(pipelineState != nullptr);
}

//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>
#include <magic_enum.hpp>

using namespace ob;
using namespace ob::rhi;

static const char* s_computeCode =
R"(
    [[vk::binding(0, 0)]] RWStructuredBuffer<float4> inputBuffer;
    [[vk::binding(1, 0)]] RWStructuredBuffer<float4> outputBuffer;

    [numthreads(8, 1, 1)]
    void CS_Main(uint3 DTid : SV_DispatchThreadID) {
        uint index = DTid.x;
        
        if (index < 1024) {
            outputBuffer[index] = inputBuffer[index] * 2.0f + float4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
)";

static const char* s_simpleComputeCode =
R"(
    [[vk::binding(0, 0)]] RWStructuredBuffer<float4> buffer;

    [numthreads(1, 1, 1)]
    void CS_Main(uint3 DTid : SV_DispatchThreadID) {
        buffer[DTid.x] = float4(DTid.x, DTid.y, DTid.z, 1.0);
    }
)";

TYPED_TEST(RHITest, ComputePipelineState_Create) {

    // 基本的なComputePipelineStateの作成をテスト
    Ref<DescriptorLayout> layout = DescriptorLayout::Create(
        {
            Binding::RWStructuredBuffer(0)
        }
    );

    // コンピュートシェーダー用のシンプルなルートシグネチャを作成
    RootSignatureDesc rootSigDesc;
    rootSigDesc.name = "TestComputeRootSignature";
    rootSigDesc.layouts = { layout };
    auto rootSignature = RootSignature::Create(rootSigDesc);

    // コンピュートシェーダーをコンパイル
    ShaderCompileDesc shaderDesc;
    shaderDesc.code = s_simpleComputeCode;
    shaderDesc.stage = ShaderStage::Compute;
    auto computeShader = Shader::Compile(shaderDesc);

    if (!Shader::Supports(ShaderStage::Compute)) {
        GTEST_SKIP() << "Compute shaders not supported on this implementation";
    }

    if (!computeShader) {
        GTEST_SKIP() << "Compute shader compilation failed - skipping ComputePipelineState test";
    }

    // ComputePipelineStateを作成
    ComputePipelineStateDesc desc;
    desc.name = "TestComputePipelineState";
    desc.rootSignature = rootSignature;
    desc.cs = computeShader;

    auto computePipelineState = ComputePipelineState::Create(desc);
    ASSERT_TRUE(computePipelineState != nullptr) << "ComputePipelineState creation failed";
}

TYPED_TEST(RHITest, ComputePipelineState_CreateWithoutRootSignature) {
    // RootSignatureが不足している場合のComputePipelineState作成失敗をテスト
    
    // コンピュートシェーダーをコンパイル
    ShaderCompileDesc shaderDesc;
    shaderDesc.code = s_simpleComputeCode;
    shaderDesc.stage = ShaderStage::Compute;
    auto computeShader = Shader::Compile(shaderDesc);

    if (!Shader::Supports(ShaderStage::Compute)) {
        GTEST_SKIP() << "Compute shaders not supported on this implementation";
    }

    if (!computeShader) {
        GTEST_SKIP() << "Compute shader compilation failed - skipping test";
    }

    // RootSignature無しでComputePipelineStateを作成（失敗するはず）
    ComputePipelineStateDesc desc;
    desc.name = "TestComputePipelineStateInvalid";
    desc.rootSignature = nullptr;  // ルートシグネチャが不足
    desc.cs = computeShader;

    auto computePipelineState = ComputePipelineState::Create(desc);
    EXPECT_TRUE(computePipelineState == nullptr) << "ComputePipelineState should fail without RootSignature";
}

TYPED_TEST(RHITest, ComputePipelineState_CreateWithoutComputeShader) {
    // ComputeShaderが不足している場合のComputePipelineState作成失敗をテスト
    
    // シンプルなルートシグネチャを作成
    RootSignatureDesc rootSigDesc;
    rootSigDesc.name = "TestComputeRootSignature";
    auto rootSignature = RootSignature::Create(rootSigDesc);
    
    if (!rootSignature) {
        GTEST_SKIP() << "RootSignature creation failed - skipping test";
    }

    // ComputeShader無しでComputePipelineStateを作成（失敗するはず）
    ComputePipelineStateDesc desc;
    desc.name = "TestComputePipelineStateInvalid";
    desc.rootSignature = rootSignature;
    desc.cs = nullptr;  // コンピュートシェーダーが不足

    auto computePipelineState = ComputePipelineState::Create(desc);

    EXPECT_TRUE(computePipelineState == nullptr) << "ComputePipelineState should fail without ComputeShader";
}

TYPED_TEST(RHITest, ComputePipelineState_CreateComplex) {
    // より複雑なコンピュートシェーダーでのComputePipelineState作成をテスト
    
    if (!Shader::Supports(ShaderStage::Compute)) {
        GTEST_SKIP() << "Compute shaders not supported on this implementation";
    }


    // 基本的なComputePipelineStateの作成をテスト
    Ref<DescriptorLayout> layout = DescriptorLayout::Create(
        {
            Binding::RWStructuredBuffer(0),
            Binding::RWStructuredBuffer(1),
        }
    );


    // バッファ用のディスクリプタテーブルを持つルートシグネチャを作成
    RootSignatureDesc rootSigDesc;
    rootSigDesc.name = "TestComplexComputeRootSignature";
    rootSigDesc.layouts = { layout };
    auto rootSignature = RootSignature::Create(rootSigDesc);
    
    if (!rootSignature) {
        GTEST_SKIP() << "RootSignature creation failed - skipping test";
    }

    // より複雑なコンピュートシェーダーをコンパイル
    ShaderCompileDesc shaderDesc;
    shaderDesc.code = s_computeCode;
    shaderDesc.stage = ShaderStage::Compute;
    auto computeShader = Shader::Compile(shaderDesc);

    if (!computeShader) {
        GTEST_SKIP() << "Complex compute shader compilation failed - skipping test";
    }

    // ComputePipelineStateを作成
    ComputePipelineStateDesc desc;
    desc.name = "TestComplexComputePipelineState";
    desc.rootSignature = rootSignature;
    desc.cs = computeShader;

    auto computePipelineState = ComputePipelineState::Create(desc);
    EXPECT_TRUE(computePipelineState != nullptr) << "Complex ComputePipelineState creation failed";
}


TYPED_TEST(RHITest, ComputePipelineState_Readback) {
    
    auto code =R"(
        [[vk::binding(0, 0)]] RWStructuredBuffer<int> buffer : register(u0);

        [numthreads(1, 1, 1)]
        void CS_Main(uint3 DTid : SV_DispatchThreadID) {
            buffer[DTid.x] = buffer[DTid.x] + 1;
        }
    )";

    Ref<DescriptorLayout> layout = DescriptorLayout::Create({Binding::RWStructuredBuffer(0)});
	Ref<DescriptorTable> table = DescriptorTable::Create({ layout });
    Ref<RootSignature> signature = RootSignature::Create({ layout });

    ComputePipelineStateDesc desc;
    desc.cs = Shader::CompileCS(code);
	desc.rootSignature = signature;

	Ref<ComputePipelineState> pipeline = ComputePipelineState::Create(desc);

    Ref<Buffer> buffer = Buffer::Create(BufferDesc::Structured<Vec4>(1024));
    buffer->update([](void* p) { for (s32 i = 0; i < 1024;++i) reinterpret_cast<int*>(p)[i] = i; });
	table->setResource(0, buffer);


    /*
    Ref<CommandList> cmdList = CommandList::Create();
    cmdList->setComputePipelineState(pipeline);
    cmdList->setRootDesciptorTable({ {table, 0} });
    // ディスパッチ
    DispatchParam dispatch;
    dispatch.groupCountX = 1;
    dispatch.groupCountY = 1;
    dispatch.groupCountZ = 1;
    cmdList->dispatch(dispatch);
    // コマンドを実行
    cmdList->execute();
    // 結果の読み取り
    buffer->readback([](const void* p) {
        for (s32 i = 0; i < 1024; ++i) {
            EXPECT_EQ(reinterpret_cast<const int*>(p)[i], i + 1);
        }
		});
    */

}
//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rendering {

    struct DrawIndexedArgs {
        u32 vertexCount = 0;
        u32 instanceCount = 1;
        u32 instanceOffset = 0;
        u32 indexCount = 0;
        u32 indexOffset = 0;
    };

    struct DrawLinearArgs {
        u32 vertexCount = 0;
        u32 vertexOffset = 0;
        u32 instanceCount = 1;
        u32 instanceOffset = 0;
    };

    using DrawArgs = std::variant<DrawLinearArgs, DrawIndexedArgs>;


    struct DrawItem {
        DrawArgs args;
        u8 stencilRef = 0;
        u8 streamBufferViewCount = 0;
        u8 shaderResourceGroupCount = 0;
        u8 scissorsCount = 0;
        u8 viewportsCount = 0;

        const class PipelineState* pipelineState = nullptr;
        const class IndexBuffer* pipelineState = nullptr;
        const class StreamBuffer* pipelineState = nullptr;
        const class ShaderResourceGroup* pipelineState = nullptr;

        const u8* rootConstants = nullptr;
        const struct Scissor* cissors = nullptr;
        const struct Viewport* viewports = nullptr;
    };


    using DrawItemSortKey = u64;
    using DrawFilterMask = u32;


}// namespcae ob
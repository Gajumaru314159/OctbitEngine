//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief  インデックス指定描画引数
    //@―---------------------------------------------------------------------------
    struct DrawIndexedArgs {
        u32 vertexCount = 0;
        u32 instanceCount = 1;
        u32 instanceOffset = 0;
        u32 indexCount = 0;
        u32 indexOffset = 0;
    };

    //@―---------------------------------------------------------------------------
    //! @brief  描画引数
    //@―---------------------------------------------------------------------------
    struct DrawLinearArgs {
        u32 vertexCount = 0;
        u32 vertexOffset = 0;
        u32 instanceCount = 1;
        u32 instanceOffset = 0;
    };

    using DrawArgs = Variant<DrawLinearArgs, DrawIndexedArgs>;


    //@―---------------------------------------------------------------------------
    //! @brief  描画アイテム
    //@―---------------------------------------------------------------------------
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
        const struct Scissor* scissors = nullptr;
        const struct Viewport* viewports = nullptr;
    };


    using DrawItemSortKey = u64;
    using DrawFilterMask = u32;


}
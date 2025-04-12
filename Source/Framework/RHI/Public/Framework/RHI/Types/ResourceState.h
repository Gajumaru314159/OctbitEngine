//***********************************************************
//! @file
//! @brief		リソース状態
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

    //! @brief      リソース状態
    //! 
    //! @details    D3D12_RESOURCE_STATE VkImageLayout VkAccessFlagBits参照
    //!             | ResourceState                     | D3D12_RESOURCE_STATE              | VkImageLayout                 |
    //!             |-----------------------------------|-----------------------------------|-------------------------------|
    //!             | Common                            | COMMON                            | UNDEFINED                     |
    //!             | VertexBuffer                      | VERTEX_AND_CONSTANT_BUFFER        | -                             |
    //!             | IndexBuffer                       | INDEX_BUFFER                      | -                             |
    //!             | ConstantBuffer                    | VERTEX_AND_CONSTANT_BUFFER        | -                             |
    //!             | UnorderedAccess                   | UNORDERED_ACCESS                  | GENERAL                       |
    //!             | PixelShadeResource                | PIXEL_SHADER_RESOURCE             | SHADER_READ_ONLY_OPTIMAL      |
    //!             | ComputeShaderResource             | -                                 | -                             |
    //!             | AllShaderResource                 | ALL_SHADER_RESOURCE               | -                             |
    //!             | StreamOut                         | STREAM_OUT                        | -                             |
    //!             | IndirectArgument                  | INDIRECT_ARGUMENT                 | -                             |
    //!             | CopyDest                          | COPY_DEST                         | TRANSFER_SRC_OPTIMAL          |
    //!             | CopySource                        | COPY_SOURCE                       | TRANSFER_DST_OPTIMAL          |
    //!             | RaytracingAccelerationStructure   | RAYTRACING_ACCELERATION_STRUCTURE | -                             |
    //!             | Present                           | PRESENT                           | PRESENT_KHR                   |
    enum class ResourceState {
		Unknown,                            //!< 不明
        Common,                             //!< 遷移元
        ConstantBuffer,                     //!< 定数バッファ
        VertexBuffer,                       //!< 頂点バッファ
        IndexBuffer,                        //!< インデックスバッファ
        IndirectArgument,                   //!< 間接引数
        ShadeResource,                      //!< シェーダーリソース
        UnorderedAccess,                    //!< アンオーダード・アクセス
		RenderTarget,                       //!< レンダーターゲット
        
        //ComputeShaderResource,              //!< コンピュートシェーダで利用可能
        //AllShaderResource,                  //!< 全てのシェーダで利用可能
        //StreamOut,                          //!< ストリーム出力先
        //CopySource,                         //!< コピー元
        //CopyDest,                           //!< コピー先
        //RaytracingAccelerationStructure,    //!< RaytracingAccelerationStructure

        ColorAttachment,
        DepthAttachment,
        DepthStencilAttachment,
        Present,                            //!< Present
        // 未対応
        // D3D12_PREDICATION
        // InputAttachment
        // ColorAttachment RW
        // DepthStencilAttachment
        // Resolve
        // Host
        // ShadingRate
        // Present
    };

}
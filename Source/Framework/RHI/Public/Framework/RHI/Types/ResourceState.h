//***********************************************************
//! @file
//! @brief		リソース状態
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      リソース使用法
    //! 
    //! @details    D3D12_RESOURCE_STATE VkImageLayout VkAccessFlagBits参照
    //!             |ResourceState                  |D3D12_RESOURCE_STATE       |VkImageLayout                    |
    //!             |-------------------------------|---------------------------|---------------------------------|
    //!             |Common                         |COMMON                     |                                 |
    //!             |VertexBuffer                   |VERTEX_AND_CONSTANT_BUFFER |-                                |
    //!             |IndexBuffer                    |INDEX_BUFFER               |-                                |
    //!             |ConstantBuffer                 |VERTEX_AND_CONSTANT_BUFFER |-                                |
    //!             |UnorderedAccess                |UNORDERED_ACCESS           |GENERAL                          |
    //!             |PixelShadeResource             |PIXEL_SHADER_RESOURCE      |SHADER_READ_ONLY_OPTIMAL         |
    //!             |ComputeShaderResource          |-                          |-                                |
    //!             |AllShaderResource              |ALL_SHADER_RESOURCE        |-                                |
    //!             |StreamOut                      |STREAM_OUT                 |-                                |
    //!             |IndirectArgument               |INDIRECT_ARGUMENT          |-                                |
    //!             |CopyDest                       |COPY_DEST                  |TRANSFER_SRC_OPTIMAL             |
    //!             |CopySource                     |COPY_SOURCE                |TRANSFER_DST_OPTIMAL             |
    //!             |RaytracingAccelerationStructure|RAYTRACING_ACCELERATION_STRUCTURE|-                          |
    //@―---------------------------------------------------------------------------
    enum class ResourceState {
        Common,                             //!< 遷移元
        VertexBuffer,                       //!< 頂点バッファ
        IndexBuffer,                        //!< インデックスバッファ
        ConstantBuffer,                     //!< 定数バッファ
        UnorderedAccess,                    //!< アンオーダード・アクセス
        PixelShadeResource,                 //!< ピクセルシェーダで利用可能
        ComputeShaderResource,              //!< コンピュートシェーダで利用可能
        AllShaderResource,                  //!< 全てのシェーダで利用可能
        StreamOut,                          //!< ストリーム出力先
        IndirectArgument,                   //!< 間接引数
        CopySource,                         //!< コピー元
        CopyDest,                           //!< コピー先
        RaytracingAccelerationStructure,    //!< RaytracingAccelerationStructure
    };

}// namespcae ob::rhi
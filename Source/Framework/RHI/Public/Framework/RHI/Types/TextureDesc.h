//***********************************************************
//! @file
//! @brief		テクスチャ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/TextureFormat.h>

namespace ob::rhi {

    //! @brief  テクスチャ・タイプ
    enum class TextureType {
        Texture1D,          //!< 1Dテクスチャ
        Texture2D,          //!< 2Dテクスチャ
        Texture3D,          //!< 3Dテクスチャ
        Cube,               //!< キューブテクスチャ
    };
    // TODO RenderTarget/DepthStencilを別管轄にする


    //! @brief  テクスチャ使用法
    enum class TextureUsage {
        Sampling,
        ColorAttachment,
        DepthAttachment,
        ResolveAttachment,
        Storage,
        StorageAttomic,
        CPURead,
        CanUpdate,
        CanCopyFrom,
        CanCopyTo,
    };
    using TextureUsages = BitFlags<TextureUsage>;


    //! @brief      テクスチャ状態
    //! 
    //! @details    D3D12_RESOURCE_STATE VkImageLayout 参照
    //!             | TextureState                      | D3D12_RESOURCE_STATE              | VkImageLayout                 |
    //!             |-----------------------------------|-----------------------------------|-------------------------------|
    //!             | Common                            | COMMON                            | -                             |
    //!             | ShadeResource                     | PIXEL_SHADER_RESOURCE             | SHADER_READ_ONLY              |
    //!             | UnorderedAccess                   | UNORDERED_ACCESS                  | GENERAL                       |
    //!             | RenderTarget                      | RENDER_TARGET                     | COLOR_ATTACHMENT              |
    //!             | DepthRead                         | DEPTH_READ                        | EPTH_STENCIL_READ_ONLY        |
    //!             | DepthWrite                        | DEPTH_WRITE                       | DEPTH_STENCIL_ATTACHMENT      |
    //!             | CopyDest                          | COPY_DEST                         | TRANSFER_SRC                  |
    //!             | CopySource                        | COPY_SOURCE                       | TRANSFER_DST                  |
    //!             | Present                           | PRESENT                           | PRESENT_SRC_KHR               |
    enum class TextureState {
        Common,
        ShaderResource,
        UnorderedAccess,
        RenderTargtet,
        DepthRead,
        DepthWrite,
        CopyDest,
        CopySource,
        Present,
    };


    enum class TextureFlag {
        ShaderResource = get_bit(0),	//!< シェーダでバインド許可
        UnorderedAccess = get_bit(1),   //!< UnorderedAccessのバインド許可
	};
    using TextureFlags = BitFlags<TextureFlag>;


    //! @brief  テクスチャ定義
    struct TextureDesc {
        String          name;
        TextureType     type        = TextureType::Texture2D;   //!< テクスチャタイプ
        TextureFormat   format      = TextureFormat::RGBA8;     //!< テクスチャフォーマット
        Size            size        = { 1,1,1 };                //!< サイズ
        s32             arrayNum    = 0;                        //!< テクスチャ配列の要素数 (Texture3Dでは0にしてください)
        s32             mipLevels   = 0;                        //!< ミップ生成レベル (0の場合sizeから自動計算されます)
		TextureFlags    flags       = TextureFlag::ShaderResource;            //!< フラグ
    };

}
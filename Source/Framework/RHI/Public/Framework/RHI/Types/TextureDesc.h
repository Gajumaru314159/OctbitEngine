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


    //! @brief  バッファ・タイプ
    enum class TextureState {
        Unknown,                //!< 指定なし
        Common,					//!< 
        ShadeResource,          //!< ピクセルシェーダで利用可能
        UnorderedAccess,        //!< アンオーダード・アクセス
        CopySource,             //!< コピー元
        CopyDest,               //!< コピー先
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
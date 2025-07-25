//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

    //! @brief  テクスチャ・フォーマット
    enum class TextureFormat {
        Unknown = -1,    //!< 不明なフォーマット
#include "TextureFormat_Color.h"
#include "TextureFormat_Depth.h"
    };


    //! @brief  カラー・テクスチャ・フォーマット
    enum class ColorTextureFormat {
        Unknown = -1,    //!< 不明なフォーマット
#include "TextureFormat_Color.h"
    };


    //! @brief  デプス・テクスチャ・フォーマット
    enum class DepthTextureFormat {
        Unknown = -1,    //!< 不明なフォーマット
#include "TextureFormat_Depth.h"
    };


    //! @brief  テクスチャ・フォーマット・ユーティリティ
    class TextureFormatUtility {
    public:

        //! @brief  HDRフォーマットか
        static bool IsHdr(TextureFormat format) {
            return
                format == TextureFormat::R10G10B10A2;
        }

        //! @brief  デプスを持っているか
        static bool HasDepth(TextureFormat format) {
            return
                format == TextureFormat::D32S8 ||
                format == TextureFormat::D32 ||
                format == TextureFormat::D24S8 ||
                format == TextureFormat::D16;
        }

        //! @brief  ステンシルを持っているか
        static bool HasStencil(TextureFormat format) {
            return
                format == TextureFormat::D32S8 ||
                format == TextureFormat::D24S8;
        }

        //! @brief  カラーを持っているか
        static bool HasColor(TextureFormat format) {
            return !(HasDepth(format) || HasStencil(format));
        }

        //! @brief  BC圧縮か
        static bool IsBC(TextureFormat format) {
            return
                format == TextureFormat::BC1 ||
                format == TextureFormat::BC2 ||
                format == TextureFormat::BC3 ||
                format == TextureFormat::BC4 ||
                format == TextureFormat::BC5 ||
                format == TextureFormat::BC6H ||
                format == TextureFormat::BC7 ||
                format == TextureFormat::BC1_SRGB ||
                format == TextureFormat::BC2_SRGB ||
                format == TextureFormat::BC3_SRGB ||
                format == TextureFormat::BC7_SRGB;
        }
    };

}
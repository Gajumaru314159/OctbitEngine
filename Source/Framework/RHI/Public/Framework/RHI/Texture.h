﻿//***********************************************************
//! @file
//! @brief		テクスチャ・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/RHI/Types/RenderTextureDesc.h>
#include <Framework/Core/Graphic/Bitmap.h>

//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      プリセットテクスチャ
    //@―---------------------------------------------------------------------------
    enum class PresetTexture{
        White,
        Gray,
        Black,
        Normal,
    };

    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャ・インターフェイス
    //@―---------------------------------------------------------------------------
    class Texture :public GraphicObject {
    public:

        static Ref<Texture> Preset(PresetTexture);

        static Ref<Texture> White();
        static Ref<Texture> Gray();
        static Ref<Texture> Black();
        static Ref<Texture> Normal();

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //! 
        //! @param desc テクスチャ定義
        //@―---------------------------------------------------------------------------
        static Ref<Texture> Create(const TextureDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //! 
        //! @param desc テクスチャ定義
        //@―---------------------------------------------------------------------------
        static Ref<Texture> Create(const Bitmap& bitmap);


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //! 
        //! @param desc テクスチャ定義
        //@―---------------------------------------------------------------------------
        static Ref<Texture> Create(Size size,Span<IntColor> colors);


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //! 
        //! @param desc テクスチャ定義
        //@―---------------------------------------------------------------------------
        static Ref<Texture> Create(BlobView blob,StringView name);


        //@―---------------------------------------------------------------------------
        //! @brief      パスからテクスチャを読み込み
        //! 
        //! @param path ファイルパス
        //@―---------------------------------------------------------------------------
        static Ref<Texture> Load(const Path& path);

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  サイズ
        //@―---------------------------------------------------------------------------
        virtual Size size()const = 0;


        //@―---------------------------------------------------------------------------
        //! @brief      幅を取得
        //@―---------------------------------------------------------------------------
        s32 Texture::width()const { return size().width; }


        //@―---------------------------------------------------------------------------
        //! @brief      高さを取得
        //@―---------------------------------------------------------------------------
        s32 Texture::height()const { return size().height; }


        //@―---------------------------------------------------------------------------
        //! @brief      深さを取得
        //@―---------------------------------------------------------------------------
        s32 Texture::depth()const { return size().depth; }


        //@―---------------------------------------------------------------------------
        //! @brief      テクスチャ・フォーマットを取得
        //@―---------------------------------------------------------------------------
        virtual TextureFormat format()const = 0;


        //@―---------------------------------------------------------------------------
        //! @brief      ミップレベルを取得
        //@―---------------------------------------------------------------------------
        virtual s32 mipLevels()const = 0;

    protected:

        virtual ~Texture() = default;

    };

}// namespace ob::rhi
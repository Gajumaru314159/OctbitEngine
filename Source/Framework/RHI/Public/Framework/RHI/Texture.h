//***********************************************************
//! @file
//! @brief		テクスチャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/TextureDesc.h>
#include <Framework/RHI/Types/BindlessHandle.h>
#include <Framework/Core/Graphic/Bitmap.h>

namespace ob::rhi {

    //! @brief      テクスチャ
    class Texture :public GraphicObject {
    public:

        static Ref<Texture> White();    //!< 白テクスチャ
        static Ref<Texture> Gray();     //!< 灰テクスチャ
        static Ref<Texture> Black();    //!< 黒テクスチャ
        static Ref<Texture> Normal();   //!< 法線テクスチャ
        static Ref<Texture> Check();    //!< チェックテクスチャ

    public:

		//! @brief  空テクスチャを生成
        static Ref<Texture> Create(const TextureDesc& desc);

		//! @brief  ビットマップデータを指定して2Dテクスチャを生成
        static Ref<Texture> Create(StringView name, const Bitmap& bitmap);

        //! @brief 色データから新しいテクスチャを作成
        //! @details 次元数はTextureTypeを指定してください。
		//!          colors.size() が size.volume() の値と一致する必要があります。
        static Ref<Texture> Create(StringView name, TextureType type,Size size,Span<const IntColor> colors);

        //! @brief  テクスチャバイナリからテクスチャを生成
        //! @details blobに渡せるデータ形式はプラットフォームごとに異なります
        //!          |プラットフォーム|形式|
        //!          |----------------|----|
        //!          |Windows         |DDS,BMP,GIF,TIFF,PNG,JPG,TGA,HDR|
        static Ref<Texture> Create(StringView name,BlobView blob);

        //! @brief      ファイルパスからテクスチャを読み込む
		//! @details    Create(StringView name,BlobView blob) のラッパー関数です。
        static Ref<Texture> Load(StringView path);

        //! @brief  ベースのテクスチャを指定して異なるビューを持つテクスチャを作成
        static Ref<Texture> Create(const TextureViewDesc& desc);


		//! @brief      テクスチャフォーマットがサポートされているか
        static bool Supports(TextureFormat format, TextureType type);

    public:

        //! @brief      名前を設定
        virtual void setName(StringView) = 0;

        //! @brief      定義取得
        virtual const TextureDesc& desc()const = 0;

        //! @brief  サイズ
        Size size()const { return desc().size; }

        //! @brief      幅を取得
        s32 width()const { return size().width; }

        //! @brief      高さを取得
        s32 height()const { return size().height; }

        //! @brief      深さを取得
        s32 depth()const { return size().depth; }

        //! @brief      深さを取得
        f32 aspectRatio()const { return 1.f * width() / height(); }

        //! @brief      テクスチャ・フォーマットを取得
        TextureFormat format()const { return desc().format; }

        //! @brief      ミップレベルを取得
        s32 mipLevels()const { return desc().mipLevels; }

		//! @brief      BindlessHandleを取得
        virtual BindlessHandle handle()const = 0;

    };

}
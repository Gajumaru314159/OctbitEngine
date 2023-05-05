//***********************************************************
//! @file
//! @brief		コマンドバッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/TextureFormat.h>
#include <Framework/Graphics/Forward.h>

namespace ob::graphics {

    //@―---------------------------------------------------------------------------
    //! @brief      テクスチャコピー領域
    //! @details    CommandBuffer::copyTexture() で使用するテクスチャの領域情報。
    //!             テクスチャ全体ではなくTexture2DArrayの特定の要素や特定のミップを
    //!             コピーする場合に使用します。
    //@―---------------------------------------------------------------------------
    struct TextureCopyRegion {
        Ref<rhi::Texture>   texture;            //!< テクスチャ
        s32                 element = -1;       //!< テクスチャ配列の何番目か(-1の場合全体)
        s32                 mipmap = -1;        //!< 何番目のミップマップか(-1の場合全体)
        IntRect             rect{-1,-1,-1,-1};  //!< テクスチャのコピー領域(-1が含まれる場合全体)
    };


    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //! 
    //! @details    
    //@―---------------------------------------------------------------------------
    class CommandBuffer:public RefObject {
    public:

        static Ref<CommandBuffer> Create();
        static Ref<CommandBuffer> Create(Ref<rhi::CommandList>& cmdList);
    public:

        //@―---------------------------------------------------------------------------
        //! @brief          バッファのすべてのコマンドをクリア
        //@―---------------------------------------------------------------------------
        virtual void clear() {}

        //@―---------------------------------------------------------------------------
        //! @brief          テクスチャを別のテクスチャにコピー
        //! @details        blitと違いコピー元とコピー先が同じピクセル数である必要があります。
        //!                 また、互換性のあるフォーマットである必要があります。
        //@―---------------------------------------------------------------------------
        //! @{
        virtual void copyTexture(const Ref<rhi::Texture>& src, const Ref<rhi::Texture>& dst) {}
        virtual void copyTexture(const TextureCopyRegion& src, const TextureCopyRegion& dst) {}
        //! @}

        //@―---------------------------------------------------------------------------
        //! @brief          カスタムシェーダを使用しテクスチャを別のものにコピー
        //! @param src      コピー元
        //! @param dest     コピー先
        //! @param mat      マテリアル
        //! @param pass     マテリアルパス名
        //@―---------------------------------------------------------------------------
        virtual void blit(const Ref<rhi::Texture>& src, const Ref<rhi::Texture>& dst, const Ref<Material>& mat = {}, Name name = {}) {}

        //@―---------------------------------------------------------------------------
        //! @brief          メッシュを描画
        //! @param mesh     描画するメッシュ
        //! @param matrix   使用するワールド行列
        //! @param material 使用するマテリアル
        //! @param submesh  描画するサブメッシュのインデックス
        //! @param pass     使用するマテリアルのパス名
        //@―---------------------------------------------------------------------------
        virtual void drawMesh(const Ref<Mesh>& mesh, s32 submesh, const Matrix& matrix, const Ref<Material>& material, Name pass = {}) {}

        //@―---------------------------------------------------------------------------
        //! @brief          メッシュを描画
        //! @param mesh     描画するメッシュ
        //! @param matrix   使用するワールド行列
        //! @param material 使用するマテリアル
        //! @param submesh  描画するサブメッシュのインデックス
        //! @param pass     使用するマテリアルのパス名
        //@―---------------------------------------------------------------------------
        virtual void drawMeshInstanced(const Ref<Mesh>& mesh, s32 submesh, Span<Matrix> matrices, const Ref<Material>& material, Name pass ={}) {}

        //@―---------------------------------------------------------------------------
        //! @brief          GPUプロファイラ用のマーカを追加
        //@―---------------------------------------------------------------------------
        virtual void pushMarker(StringView) {}

        //@―---------------------------------------------------------------------------
        //! @brief          GPUプロファイラ用のマーカを追加
        //@―---------------------------------------------------------------------------
        virtual void popMarker() {}

    public:
        // 実装未定の機能

        virtual void clearRenderTarget(Optional<Color> clearColor, Optional<f32> depth) {}

        virtual void setScissorRect(const IntRect& rect){}
        virtual void setViewport(const IntRect& rect){}

        virtual void drawMeshInstancedIndirect(const Ref<Mesh>& mesh, s32 submesh, const Ref<Material>& material, Name pass,const Ref<rhi::Buffer>& buffer,s32 argsOffset) {}
        virtual void generatteMipmaps(const Ref<rhi::Texture>&){}


        virtual void getTemporaryRT(Name name, s32 width, s32 height, rhi::TextureFormat format) {}
        virtual void releaseTemporaryRT(Name name){}

        virtual void setRenderTarget(Span<Ref<rhi::RenderTexture>>) {}
    };


}
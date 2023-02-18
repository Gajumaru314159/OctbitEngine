//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Name.h>
#include <Framework/RHI/Forward.h>

namespace ob::graphic {

    class Material;
    class Mesh;
    
    using Name = engine::Name;

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //! 
    //! @details    
    //@―---------------------------------------------------------------------------
    class CommandBuffer:public RefObject {
    public:

        static Ref<CommandBuffer> Create();
    public:

        //@―---------------------------------------------------------------------------
        //! @brief          バッファのすべてのコマンドをクリア
        //@―---------------------------------------------------------------------------
        virtual void clear()=0;

        //@―---------------------------------------------------------------------------
        //! @brief          カスタムシェーダを使用しテクスチャを別のものにコピー
        //! @param src      コピー元
        //! @param dest     コピー先
        //! @param mat      マテリアル
        //! @param pass     マテリアルパス名
        //@―---------------------------------------------------------------------------
        virtual void blit(const Ref<rhi::Texture>& src, const Ref<rhi::Texture>& dst, const Ref<Material>& mat = {}, Name name = {}) = 0;

        //@―---------------------------------------------------------------------------
        //! @brief          メッシュを描画
        //! @param mesh     描画するメッシュ
        //! @param matrix   使用するワールド行列
        //! @param material 使用するマテリアル
        //! @param submesh  描画するサブメッシュのインデックス
        //! @param pass     使用するマテリアルのパス名
        //@―---------------------------------------------------------------------------
        virtual void drawMesh(const Ref<Mesh>& mesh, s32 submesh, const Matrix& matrix, const Ref<Material>& material, Name pass = {}) = 0;

        //@―---------------------------------------------------------------------------
        //! @brief          メッシュを描画
        //! @param mesh     描画するメッシュ
        //! @param matrix   使用するワールド行列
        //! @param material 使用するマテリアル
        //! @param submesh  描画するサブメッシュのインデックス
        //! @param pass     使用するマテリアルのパス名
        //@―---------------------------------------------------------------------------
        virtual void drawMeshInstanced(const Ref<Mesh>& mesh, s32 submesh, Span<Matrix> matrices, const Ref<Material>& material, Name pass ={}) = 0;

        //@―---------------------------------------------------------------------------
        //! @brief          GPUプロファイラ用のマーカを追加
        //@―---------------------------------------------------------------------------
        virtual void pushMarker(StringView) = 0;

        //@―---------------------------------------------------------------------------
        //! @brief          GPUプロファイラ用のマーカを追加
        //@―---------------------------------------------------------------------------
        virtual void popMarker() = 0;

    };


}// namespcae ob
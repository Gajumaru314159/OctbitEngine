﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/CommandBuffer.h>

namespace ob::graphic
{


    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //! 
    //! @details    
    //@―---------------------------------------------------------------------------
    class CommandBufferImpl :public CommandBuffer {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief          バッファのすべてのコマンドをクリア
        //@―---------------------------------------------------------------------------
        void clear() override;

        //@―---------------------------------------------------------------------------
        //! @brief          カスタムシェーダを使用しテクスチャを別のものにコピー
        //! @param src      コピー元
        //! @param dest     コピー先
        //! @param mat      マテリアル
        //! @param pass     マテリアルパス名
        //@―---------------------------------------------------------------------------
        void blit(const Ref<Texture>& src, const Ref<Texture>& dst, const Ref<Material>& mat, Name name)  override;

        //@―---------------------------------------------------------------------------
        //! @brief          メッシュを描画
        //! @param mesh     描画するメッシュ
        //! @param matrix   使用するワールド行列
        //! @param material 使用するマテリアル
        //! @param submesh  描画するサブメッシュのインデックス
        //! @param pass     使用するマテリアルのパス名
        //@―---------------------------------------------------------------------------
        void drawMesh(const Ref<Mesh>& mesh,s32 submesh, const Matrix& matrix, const Ref<Material>& material, Name pass)  override;

        //@―---------------------------------------------------------------------------
        //! @brief          メッシュを描画
        //! @param mesh     描画するメッシュ
        //! @param matrix   使用するワールド行列
        //! @param material 使用するマテリアル
        //! @param submesh  描画するサブメッシュのインデックス
        //! @param pass     使用するマテリアルのパス名
        //@―---------------------------------------------------------------------------
        void drawMeshInstanced(const Ref<Mesh>& mesh, s32 submesh, Span<Matrix> matrices, const Ref<Material>& material, Name pass)  override;

        //@―---------------------------------------------------------------------------
        //! @brief          GPUプロファイラ用のマーカを追加
        //@―---------------------------------------------------------------------------
        void pushMarker(StringView)  override;

        //@―---------------------------------------------------------------------------
        //! @brief          GPUプロファイラ用のマーカを追加
        //@―---------------------------------------------------------------------------
        void popMarker()  override;

    private:

        Ref<rhi::CommandList> m_cmdList;

    };

}// namespace ob
//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/CommandBuffer/CommandBufferImpl.h>
#include <Framework/Graphic/Material/MaterialImpl.h>
#include <Framework/Graphic/Mesh/MeshImpl.h>

namespace ob::graphic
{

    //@―---------------------------------------------------------------------------
    //! @brief          バッファのすべてのコマンドをクリア
    //@―---------------------------------------------------------------------------
    void CommandBufferImpl::clear() {
        OB_NOTIMPLEMENTED();
    }

    //@―---------------------------------------------------------------------------
    //! @brief          カスタムシェーダを使用しテクスチャを別のものにコピー
    //! @param src      コピー元
    //! @param dest     コピー先
    //! @param mat      マテリアル
    //! @param pass     マテリアルパス名
    //@―---------------------------------------------------------------------------
    void CommandBufferImpl::blit(const Ref<Texture>& src, const Ref<Texture>& dst, const Ref<Material>& mat = {}, Name name = {}) {
        OB_NOTIMPLEMENTED();
    }

    //@―---------------------------------------------------------------------------
    //! @brief          メッシュを描画
    //! @param mesh     描画するメッシュ
    //! @param matrix   使用するワールド行列
    //! @param material 使用するマテリアル
    //! @param submesh  描画するサブメッシュのインデックス
    //! @param pass     使用するマテリアルのパス名
    //@―---------------------------------------------------------------------------
    void CommandBufferImpl::drawMesh(const Ref<Mesh>& mesh, s32 submesh, const Matrix& matrix, const Ref<Material>& material, Name pass = {}) {
        
        if (auto pMat = material.cast<MaterialImpl>()) {
            pMat->record(m_cmdList, matrix,mesh,submesh, pass);
        }

    }

    //@―---------------------------------------------------------------------------
    //! @brief          メッシュを描画
    //! @param mesh     描画するメッシュ
    //! @param matrix   使用するワールド行列
    //! @param material 使用するマテリアル
    //! @param submesh  描画するサブメッシュのインデックス
    //! @param pass     使用するマテリアルのパス名
    //@―---------------------------------------------------------------------------
    void CommandBufferImpl::drawMeshInstanced(const Ref<Mesh>& mesh, s32 submesh, Span<Matrix> matrices, const Ref<Material>& material, Name pass = {}) {
        OB_NOTIMPLEMENTED();
    }

    //@―---------------------------------------------------------------------------
    //! @brief          GPUプロファイラ用のマーカを追加
    //@―---------------------------------------------------------------------------
    void CommandBufferImpl::pushMarker(StringView name) {
        m_cmdList->pushMarker(name);
    }

    //@―---------------------------------------------------------------------------
    //! @brief          GPUプロファイラ用のマーカを追加
    //@―---------------------------------------------------------------------------
    void CommandBufferImpl::popMarker() {
        m_cmdList->popMarker();
    }

}// namespace ob
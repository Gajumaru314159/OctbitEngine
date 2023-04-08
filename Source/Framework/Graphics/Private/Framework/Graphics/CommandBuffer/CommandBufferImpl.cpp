//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/CommandBuffer/CommandBufferImpl.h>
#include <Framework/Graphics/Material/MaterialImpl.h>
#include <Framework/Graphics/Mesh/MeshImpl.h>
#include <Framework/RHI/CommandList.h>

namespace ob::graphics
{

    //@―---------------------------------------------------------------------------
    //! @brief          生成
    //@―---------------------------------------------------------------------------
    Ref<CommandBuffer> CommandBuffer::Create() {
        return new CommandBufferImpl();
    }
    //@―---------------------------------------------------------------------------
    //! @brief          生成
    //@―---------------------------------------------------------------------------
    Ref<CommandBuffer> CommandBuffer::Create(Ref<rhi::CommandList>& cmdList) {
        return new CommandBufferImpl(cmdList);
    }

    //@―---------------------------------------------------------------------------
    //! @brief          コンストラクタ
    //@―---------------------------------------------------------------------------
    CommandBufferImpl::CommandBufferImpl() {
        rhi::CommandListDesc desc;
        desc.name = TC("CommandBuffer");
        desc.type = rhi::CommandListType::Graphic;
        m_cmdList = rhi::CommandList::Create(desc);

        m_cmdList->begin();
    }

    //@―---------------------------------------------------------------------------
    //! @brief          コンストラクタ
    //@―---------------------------------------------------------------------------
    CommandBufferImpl::CommandBufferImpl(Ref<rhi::CommandList>& cmdList) {
        m_cmdList = cmdList;
    }


    //@―---------------------------------------------------------------------------
    //! @brief          バッファのすべてのコマンドをクリア
    //@―---------------------------------------------------------------------------
    void CommandBufferImpl::clear() {
        if (!m_cmdList)
            return;
        // いらない？
        OB_NOTIMPLEMENTED();
    }

    //@―---------------------------------------------------------------------------
    //! @brief          カスタムシェーダを使用しテクスチャを別のものにコピー
    //! @param src      コピー元
    //! @param dest     コピー先
    //! @param mat      マテリアル
    //! @param pass     マテリアルパス名
    //@―---------------------------------------------------------------------------
    void CommandBufferImpl::blit(const Ref<rhi::Texture>& src, const Ref<rhi::Texture>& dst, const Ref<Material>& mat = {}, Name name = {}) {
        if (!m_cmdList)
            return;
        // BuiltInMaterial::Blit

        //auto table = rhi::DescriptorTable::Create(rhi::DescriptorHeapType::CBV_SRV_UAV, 1);
        //table->setResource(0, src);

        //m_cmdList->

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
        if (!m_cmdList)
            return;

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
        if (!m_cmdList)
            return;
        OB_NOTIMPLEMENTED();
    }

    //@―---------------------------------------------------------------------------
    //! @brief          GPUプロファイラ用のマーカを追加
    //@―---------------------------------------------------------------------------
    void CommandBufferImpl::pushMarker(StringView name) {
        if (!m_cmdList)
            return;
        m_cmdList->pushMarker(name);
    }

    //@―---------------------------------------------------------------------------
    //! @brief          GPUプロファイラ用のマーカを追加
    //@―---------------------------------------------------------------------------
    void CommandBufferImpl::popMarker() {
        if (!m_cmdList)
            return;
        m_cmdList->popMarker();
    }

}// namespace ob
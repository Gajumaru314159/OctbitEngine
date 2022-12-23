//***********************************************************
//! @file
//! @brief		リソースステート・キャッシュ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  リソースステート・キャッシュ
    //@―---------------------------------------------------------------------------
    class ResourceStateCache {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        ResourceStateCache() {

        }

        void addBuffer(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) {

            OB_NOTIMPLEMENTED();

        }

        void addTexture(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after, s32 subresourceIndex) {

            addTransition(resource, before, after);

        }

        //@―---------------------------------------------------------------------------
        //! @brief  コマンドを記録
        //@―---------------------------------------------------------------------------
        void recordCommand(ID3D12GraphicsCommandList& commandList) {

            if (m_barriers.empty()) {
                return;
            }

            commandList.ResourceBarrier((UINT)m_barriers.size(), m_barriers.data());

            m_barriers.clear();

            clear();

        }

        //@―---------------------------------------------------------------------------
        //! @brief  追加済みのバリアをコマンドリストに記録
        //@―---------------------------------------------------------------------------
        void clear() {
            m_barriers.clear();
        }

    private:

        //@―---------------------------------------------------------------------------
        //! @brief  リソースのステート遷移を追加
        //@―---------------------------------------------------------------------------
        void addTransition(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) {

            if (before == after) {

                if (before == D3D12_RESOURCE_STATE_UNORDERED_ACCESS) {

                    auto& desc = m_barriers.emplace_back();
                    desc.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
                    desc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                    desc.UAV.pResource = resource;

                }

                return;
            }

            auto& desc = m_barriers.emplace_back();
            desc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            desc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            desc.Transition.pResource = resource;
            desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            desc.Transition.StateBefore = before;
            desc.Transition.StateAfter = after;

        }

        //@―---------------------------------------------------------------------------
        //! @brief  リソースのエイリアシング遷移を追加
        //@―---------------------------------------------------------------------------
        void addTransition(ID3D12Resource* before, ID3D12Resource* after) {

            auto& desc = m_barriers.emplace_back();
            desc.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
            desc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            desc.Aliasing.pResourceBefore = before;
            desc.Aliasing.pResourceAfter = after;

        }

    private:

        Array<D3D12_RESOURCE_BARRIER> m_barriers;

    };

}// ob::rhi::dx12 
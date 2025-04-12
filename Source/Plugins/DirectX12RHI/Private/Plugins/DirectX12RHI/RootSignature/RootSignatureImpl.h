//***********************************************************
//! @file
//! @brief		ルートシグネチャ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/Forward.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi::dx12 {
    class DirectX12RHI;
    class ITexture;
}


//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi::dx12 {

    //! @brief  ルートシグネチャ実装(DirectX12)
    class RootSignatureImpl :public rhi::RootSignature {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //! @brief  コンストラクタ
        RootSignatureImpl(DirectX12RHI& rDevice, const BindingLayoutDesc& desc);


        //! @brief  デストラクタ
        ~RootSignatureImpl();


        //! @brief  妥当なオブジェクトか
        bool isValid()const;

        
        //! @brief      名前を取得
        const String& getName()const override;


        //===============================================================
        // ゲッター
        //===============================================================

        //! @brief  定義を取得
        const BindingLayoutDesc& getDesc()const noexcept override;


        //! @brief  ネイティブオブジェクトを取得
        auto getNative()const noexcept { return m_rootSignature.Get(); };

        s32 getItemCount(s32 slot) const {
            if (!is_in_range(slot, m_desc.slots)) return false;
            return m_desc.slots[slot].items.size();
        }

		bool isSampler(s32 slot)const {
            if (!is_in_range(slot, m_desc.slots)) return false;
            if (m_desc.slots[slot].items.empty()) return false;
			return m_desc.slots[slot].items.front().type == BindingType::Sampler;
		}

    private:
        BindingLayoutDesc m_desc;

        ComPtr<ID3D12RootSignature> m_rootSignature;    //!< ルートシグネチャ

    };

}
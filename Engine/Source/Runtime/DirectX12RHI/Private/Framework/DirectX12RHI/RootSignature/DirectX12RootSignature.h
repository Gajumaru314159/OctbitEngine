//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/RootSignature.h>
#include <Framework/RHI/Forward.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi {
    class DirectX12Device;
    class ITexture;
}


//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi {

    //! @brief  ルートシグネチャ実装(DirectX12)
    class DirectX12RootSignature :public rhi::RootSignature {
    public:
        struct MapInfo {
            s32 samplerSlot = -1;
            s32 othersSlot = -1;
        };
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //! @brief  コンストラクタ
        DirectX12RootSignature(DirectX12Device& device, const RootSignatureDesc& desc);


        //! @brief  デストラクタ
        ~DirectX12RootSignature() override;


        //! @brief  妥当なオブジェクトか
        bool isValid()const;

        
        //! @brief      名前を取得
        const String& getName()const override;


        //===============================================================
        // ゲッター
        //===============================================================

        //! @brief  定義を取得
        const RootSignatureDesc& getDesc()const noexcept override;


        //! @brief  ネイティブオブジェクトを取得
        auto getNative()const noexcept { return m_rootSignature.Get(); };

		MapInfo getMapInfo(s32 index)const {
			return m_mapInfos.at(index);
		}

    private:
        RootSignatureDesc m_desc;

        ComPtr<ID3D12RootSignature> m_rootSignature;    //!< ルートシグネチャ
		Vector<MapInfo> m_mapInfos;                //!< バインドスロット情報
    };

}
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/DescriptorLayout.h>
#include <Plugins/DirectX12RHI/Descriptor/DescriptorHeapType.h>

namespace ob::rhi {

	class DirectX12Device;

    //! @brief  デスクリプタ・レイアウト実装(DirectX12)
	//! @details DirectX12では対応するオブジェクトが存在しません。RootSignature/DescirtorTable生成時に必要な情報を保持します。
    class DirectX12DescriptorLayout :public DescriptorLayout {
	public:
		struct HeapInfo {
			s32 samplerNum;
			s32 othersNum;
		};
		struct MapInfo {
			DescriptorHeapType	type;
			u32					index;
		};
    public:

        //! @brief              コンストラクタ
		DirectX12DescriptorLayout(const DescriptorLayoutDesc& desc);
		

		//! @brief  妥当な状態か
		bool isValid()const { return true; }


		//! @brief      名前を取得
		const String& getName()const override { return m_desc.name; }


		const DescriptorLayoutDesc& getDesc() const override { return m_desc; }

		const MapInfo& getMapInfo(s32 index) const {
			return m_mapInfos.at(index);
		}
		const HeapInfo& getHeapInfo() const {
			return m_heapInfo;
		}

    private:
		DescriptorLayoutDesc	m_desc;
		Vector<MapInfo>			m_mapInfos;	
		HeapInfo				m_heapInfo;
    };

}
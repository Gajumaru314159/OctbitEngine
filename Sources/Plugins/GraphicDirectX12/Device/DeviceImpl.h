﻿//***********************************************************
//! @file
//! @brief		デバイス実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/IDevice.h>
#include <Framework/Graphic/Types/FeatureLevel.h>
#include <Framework/Graphic/Types/DescriptorDesc.h>

//===============================================================
// クラス定義
//===============================================================
namespace ob::graphic::dx12 {

	class DeviceImpl :public IDevice {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		DeviceImpl(FeatureLevel featureLevel);


		//@―---------------------------------------------------------------------------
		//! @brief  妥当な状態か
		//@―---------------------------------------------------------------------------
		bool isValid()const override;


		//===============================================================
		// 更新
		//===============================================================

		void entryCommandList(const class CommandList&) override;
		void update()override;


		//===============================================================
		// 生成
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  スワップ・チェーンを生成
		//@―---------------------------------------------------------------------------
		ob::graphic::ISwapChain* createSwapChain(const SwapchainDesc& desc)override;


		//@―---------------------------------------------------------------------------
		//! @brief  コマンドリスト生成
		//@―---------------------------------------------------------------------------
		ob::graphic::ICommandList* createCommandList(const CommandListDesc& desc)override;


		//@―---------------------------------------------------------------------------
		//! @brief  ルートシグネチャを生成
		//@―---------------------------------------------------------------------------
		ob::graphic::IRootSignature* createRootSignature(const RootSignatureDesc& desc)override;


		//@―---------------------------------------------------------------------------
		//! @brief  パイプラインステートを生成
		//@―---------------------------------------------------------------------------
		ob::graphic::IPipelineState* createPipelineState(const PipelineStateDesc& desc)override;


		//@―---------------------------------------------------------------------------
		//! @brief  テクスチャを生成
		//@―---------------------------------------------------------------------------
		ob::graphic::ITexture* createTexture(const TextureDesc& desc)override;


		//@―---------------------------------------------------------------------------
		//! @brief  レンダーターゲットを生成
		//@―---------------------------------------------------------------------------
		ob::graphic::IRenderTarget* createRenderTarget(const RenderTargetDesc& desc)override;


		//@―---------------------------------------------------------------------------
		//! @brief  バッファーを生成
		//@―---------------------------------------------------------------------------
		ob::graphic::IBuffer* createBuffer(const BufferDesc& desc) override;


		//@―---------------------------------------------------------------------------
		//! @brief  シェーダを生成
		//@―---------------------------------------------------------------------------
		ob::graphic::IShader* createShader(const String&, ShaderStage) override;


		//@―---------------------------------------------------------------------------
		//! @brief  シェーダを生成
		//@―---------------------------------------------------------------------------
		ob::graphic::IShader* createShader(const Blob&, ShaderStage) override;


		//@―---------------------------------------------------------------------------
		//! @brief  デスクリプタ・テーブルを生成
		//@―---------------------------------------------------------------------------
		ob::graphic::IDescriptorTable* createDescriptorTable(DescriptorHeapType type, s32 elementNum)override;

	public:

		//===============================================================
		// ゲッター
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  ネイティブ・デバイスを取得
		//@―---------------------------------------------------------------------------
		ComPtr<ID3D12Device8>& getNative();


		//@―---------------------------------------------------------------------------
		//! @brief  ファクトリを取得
		//@―---------------------------------------------------------------------------
		ComPtr<IDXGIFactory7>& getFactory();


		//@―---------------------------------------------------------------------------
		//! @brief  システム・コマンド・キューを取得
		//@―---------------------------------------------------------------------------
		ComPtr<ID3D12CommandQueue>& getCommandQueue();


		//@―---------------------------------------------------------------------------
		//! @brief  システム・コマンド・リストを取得
		//@―---------------------------------------------------------------------------
		//ComPtr<ID3D12GraphicsCommandList>& getSystemCommandList();


		//@―---------------------------------------------------------------------------
		//! @brief          ハンドルをアロケート
		//! 
		//! @param type     ヒープタイプ
		//! @param handle   アロケート先ハンドル
		//! @param size     割り当て個数
		//@―---------------------------------------------------------------------------
		void allocateHandle(DescriptorHeapType type, class DescriptorHandle& handle, s32 size);


		//@―---------------------------------------------------------------------------
		//! @brief          デスクリプタヒープを設定
		//@―---------------------------------------------------------------------------
		void setDescriptorHeaps(class CommandListImpl& cmdList);


	private:

		bool initialize();

		bool initializeDXGIDevice();
		bool initializeCommand();
		bool initializeFence();
		bool initializeVideoCardInfo();
		bool initializeDescriptorHeaps();

	private:

		FeatureLevel                        m_featureLevel;             // フィーチャーレベル
		ComPtr<ID3D12Device8>               m_device;                   // D3D12のデバイス本体
		ComPtr<IDXGIFactory7>               m_dxgiFactory;              // DXGIインターフェイス

		ComPtr<ID3D12CommandAllocator>      m_commandAllocator;         // コマンドアロケータ
		//ComPtr<ID3D12CommandQueue>          m_commandQueue;             // コマンドキュー
		//ComPtr<ID3D12GraphicsCommandList>   m_systemCmdList;            // システムコマンドリスト

		std::unique_ptr<class CommandQueue>	m_commandQueue;

		ComPtr<ID3D12Fence>                 m_fence;
		UINT64                              m_fenceVal;


		vector<std::unique_ptr<class DescriptorHeap>>        m_descriptorHeaps;          // デスクリプタ・ヒープ・リスト

#ifdef OB_DEBUG
		std::unique_ptr<class PIXModule> m_pixModule;
#endif
	};
}// namespace ob::graphic::dx12






//===============================================================
// インライン
//===============================================================
namespace ob::graphic::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief  ネイティブ・デバイスを取得
	//@―---------------------------------------------------------------------------
	inline ComPtr<ID3D12Device8>& DeviceImpl::getNative() {
		return m_device;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ファクトリを取得
	//@―---------------------------------------------------------------------------
	inline ComPtr<IDXGIFactory7>& DeviceImpl::getFactory() {
		return m_dxgiFactory;
	}

}// namespace ob::graphic::dx12
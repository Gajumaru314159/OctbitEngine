﻿//***********************************************************
//! @file
//! @brief		RHI・モジュール(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/Config.h>
#include <Framework/RHI/Types/DescriptorDesc.h>
#include <Framework/Core/Utility/Pimpl.h>

namespace ob::platform {
	class WindowManager;
}

namespace ob::rhi::dx12 {

	class DirectX12RHI : public RHI{
	public:
		DirectX12RHI(ob::rhi::Config*,ob::platform::WindowManager&);
		~DirectX12RHI();

		//@―---------------------------------------------------------------------------
		//! @brief  ゲーム更新イベント
		//@―---------------------------------------------------------------------------
		void update() override;


		//@―---------------------------------------------------------------------------
		//! @brief  妥当な状態か
		//@―---------------------------------------------------------------------------
		bool isValid()const override;


		//===============================================================
		// 更新
		//===============================================================

		void entryCommandList(const CommandList&) override;


		//===============================================================
		// 生成
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  レンダーパス生成
		//@―---------------------------------------------------------------------------
		Ref<RenderPass> createRenderPass(const RenderPassDesc& desc)override;

		//@―---------------------------------------------------------------------------
		//! @brief  フレームバッファを生成
		//@―---------------------------------------------------------------------------
		Ref<FrameBuffer> createFrameBuffer(const FrameBufferDesc& desc)override;

		//@―---------------------------------------------------------------------------
		//! @brief  スワップ・チェーンを生成
		//@―---------------------------------------------------------------------------
		Ref<Display> createDisplay(const DisplayDesc& desc)override;


		//@―---------------------------------------------------------------------------
		//! @brief  コマンドリスト生成
		//@―---------------------------------------------------------------------------
		Ref<CommandList> createCommandList(const CommandListDesc& desc)override;


		//@―---------------------------------------------------------------------------
		//! @brief  ルートシグネチャを生成
		//@―---------------------------------------------------------------------------
		Ref<RootSignature> createRootSignature(const RootSignatureDesc& desc)override;


		//@―---------------------------------------------------------------------------
		//! @brief  パイプラインステートを生成
		//@―---------------------------------------------------------------------------
		Ref<PipelineState> createPipelineState(const PipelineStateDesc& desc)override;


		//@―---------------------------------------------------------------------------
		//! @brief  テクスチャを生成
		//@―---------------------------------------------------------------------------
		Ref<Texture> createTexture(const TextureDesc& desc)override;


		Ref<Texture> createTexture(Size size, Span<IntColor> colors) override;


		//@―---------------------------------------------------------------------------
		//! @brief  テクスチャを生成
		//@―---------------------------------------------------------------------------
		Ref<Texture> createTexture(BlobView blob, StringView name)override;


		//@―---------------------------------------------------------------------------
		//! @brief  レンダーテクスチャを生成
		//@―---------------------------------------------------------------------------
		Ref<RenderTexture> createRenderTexture(const RenderTextureDesc& desc)override;


		//@―---------------------------------------------------------------------------
		//! @brief  バッファーを生成
		//@―---------------------------------------------------------------------------
		Ref<Buffer> createBuffer(const BufferDesc& desc) override;


		//@―---------------------------------------------------------------------------
		//! @brief  シェーダを生成
		//@―---------------------------------------------------------------------------
		Ref<Shader> createShader(const String&, ShaderStage) override;


		//@―---------------------------------------------------------------------------
		//! @brief  シェーダを生成
		//@―---------------------------------------------------------------------------
		Ref<Shader> createShader(BlobView, ShaderStage) override;


		//@―---------------------------------------------------------------------------
		//! @brief  デスクリプタ・テーブルを生成
		//@―---------------------------------------------------------------------------
		Ref<DescriptorTable> createDescriptorTable(DescriptorHeapType type, s32 elementNum)override;


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
		bool initializeVideoCardInfo();
		bool initializeDescriptorHeaps();

	private:

		Config								m_config;

		ComPtr<ID3D12Device8>               m_device;                   // D3D12のデバイス本体
		ComPtr<IDXGIFactory7>               m_dxgiFactory;              // DXGIインターフェイス

		UPtr<class CommandQueue>			m_commandQueue;

		ComPtr<ID3D12Fence>                 m_fence;
		UINT64                              m_fenceVal;


		HashMap<DescriptorHeapType, UPtr<class DescriptorHeap>>        m_descriptorHeaps;          // デスクリプタ・ヒープ・リスト



#ifdef OB_DEBUG
		UPtr<class PIXModule> m_pixModule;
#endif

		//Pimpl<DirectX12RHI> m_device;// DirectX12RHI内にGraphicObjectはないので後から解放
		Pimpl<GraphicObjectManager> m_objectManager;
	};

}
//===============================================================
// インライン
//===============================================================
namespace ob::rhi::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief  ネイティブ・デバイスを取得
	//@―---------------------------------------------------------------------------
	inline ComPtr<ID3D12Device8>& DirectX12RHI::getNative() {
		return m_device;
	}


	//@―---------------------------------------------------------------------------
	//! @brief  ファクトリを取得
	//@―---------------------------------------------------------------------------
	inline ComPtr<IDXGIFactory7>& DirectX12RHI::getFactory() {
		return m_dxgiFactory;
	}

}// namespace ob::rhi::dx12
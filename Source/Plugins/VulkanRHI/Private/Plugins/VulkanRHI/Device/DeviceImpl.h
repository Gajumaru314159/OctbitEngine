//***********************************************************
//! @file
//! @brief		デバイス実装(Vulkan)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/IDevice.h>
#include <Framework/Graphic/Types/FeatureLevel.h>
#include <Framework/Graphic/Types/DescriptorDesc.h>

#include <Plugins/VulkanRHI/Device/DebugReportCallBack.h>
#include <Plugins/VulkanRHI/Device/Instance.h>
#include <Plugins/VulkanRHI/Device/PhysicalDevice.h>

//===============================================================
// クラス定義
//===============================================================
namespace ob::graphic::vulkan {

	class DeviceImpl :public IDevice {
	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		DeviceImpl(FeatureLevel featureLevel);
		~DeviceImpl();


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
		//! @brief  テクスチャを生成
		//@―---------------------------------------------------------------------------
		ob::graphic::ITexture* createTexture(BlobView desc)override;


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

	private:

		bool initialize();

	private:

		FeatureLevel                        m_featureLevel;             // フィーチャーレベル

		UPtr<Instance> m_instance;
		//UPtr<DebugReportCallback> m_callback;

		VkPhysicalDevice m_physicalDevice = nullptr;
		VkPhysicalDeviceMemoryProperties m_physMemProps;
		VkDevice m_device;
	};
}// namespace ob::graphic::dx12






//===============================================================
// インライン
//===============================================================
namespace ob::graphic::dx12 {

}// namespace ob::graphic::dx12
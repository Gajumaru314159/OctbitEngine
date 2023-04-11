//***********************************************************
//! @file
//! @brief		RHI・モジュール(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/RHI.h>
#include <Framework/RHI/Texture.h>

namespace ob::rhi {

	class SystemResourceModule : public Singleton<SystemResourceModule> {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		SystemResourceModule(RHI&);

		//@―---------------------------------------------------------------------------
		//! @brief  デストラクタ
		//@―---------------------------------------------------------------------------
		~SystemResourceModule();

		//@―---------------------------------------------------------------------------
		//! @brief  プリセットテクスチャ取得
		//@―---------------------------------------------------------------------------
		Ref<Texture> getPresetTexture(PresetTexture type);

	private:

		HashMap<PresetTexture, Ref<Texture>> m_presetTextures;

	};

}// namespace ob::rhi::dx12

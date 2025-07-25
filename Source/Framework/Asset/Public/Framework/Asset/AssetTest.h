//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Asset/Asset.h>
#include <Framework/Asset/AssetHandle.h>
#include <Framework/Asset/AssetFactory.h>

namespace ob::asset {

	class TextureAsset : public Asset {
	public:
		void getWidth() const {}
	};
	using TextureAssetHandle = AssetHandle<TextureAsset>;




	class TextureAssetFactory : public AssetFactory {
	public:
		SPtr<Asset> create() {
			return {};
		}
	};

	class SampleComponent {
	public:
		void setAsset(const AssetId& path) {

			m_hTexture.load(
				path,
				[&]() {
					if (m_hTexture.isReady()) {
						m_hTexture->getWidth();
					}
				}
			);

			SPtr<TextureAsset> a;
			a->getId();

		}
	private:
		TextureAssetHandle m_hTexture;
	};

}
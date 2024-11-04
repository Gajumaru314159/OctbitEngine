//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Asset/AssetTypes.h>
#include <Framework/Asset/AssetId.h>
#include <Framework/Asset/AssetFactory.h>

namespace ob::asset {


	class AssetLoader {
	public:
		AssetLoader(const AssetId& id, AssetFactory& factory)
			: m_id(id)
			, m_factory(factory)
		{
		}

		void load() {
			Char path[256];
			FormatTo(path, "Binaries/{}/asset.bin", m_id);

			File file(path);

			auto asset = m_factory.create();

			//m_loadNotifier.invoke(asset);

		}

		void swap() {

		}

		void addLoadEvent(AssetEventHandle& handle, AssetEventDelegate&& d) {
			m_loadNotifier.add(handle, std::move(d));
		}
	private:
		void onLoad() {
			SPtr<Asset> asset;
			m_loadNotifier.invoke(asset);
		}
	private:
		AssetId m_id;
		AssetFactory& m_factory;
		AssetEventNotifier m_loadNotifier;
	};

}
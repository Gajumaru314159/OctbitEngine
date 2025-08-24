//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Asset/AssetTypes.h>
#include <Framework/Asset/AssetFactory.h>

#include <Framework/Asset/Asset.h>
#include <Framework/Core/Thread/ThreadPool.h>
#include <Framework/Core/Misc/Locale.h>

namespace ob::asset {

	using AssetId = UUID;

	class AssetLoader {
	public:
		AssetLoader(const AssetId& id, AssetFactory& factory)
			: m_id(id)
			, m_factory(factory)
		{
		}

		//! @brief AssetId を取得
		const String& getAssetType() const {
			return m_assetType;
		}

		void loadAsync(Locale locale) {

			// 依存アセットもロード命令
			for (auto& depenency : m_dependencies) {

			}



			Char path[256];
			FormatTo(path, "Binaries/{}/asset.bin", m_id);

			File file(path);

			// バイナリに問題がありアセットの生成に失敗した場合はnullが返ってくる。
			auto asset = m_factory.create();
			{
				ScopeLock lock(m_assetLock);

				// いつ参照を外せるのか？
				m_asset = asset;
			}

			// swapper.enqueue();

			// 依存アセットが全て読み込まれるまでタスクを積みなおし
		}

		void swap() {
			// NOTE auto asset = m_asset; としてからinvokeしたほうが良い？
			ScopeLock lock(m_assetLock);
			m_loadNotifier.invoke(m_asset);
		}

		void addLoadEvent(AssetEventHandle& handle, AssetEventDelegate&& d) {
			ScopeLock lock(m_assetLock);
			if (m_asset) {
				d(m_asset);
			}
			m_loadNotifier.add(handle, std::move(d));
		}
	private:
		const String m_assetType;
		AssetId m_id;
		AssetFactory& m_factory;
		AssetEventNotifier m_loadNotifier;

		// ロード中に新規読み込みリクエストが来た場合は？
		SpinLock m_assetLock;
		Ref<Asset> m_asset;

		// TODO 実体を持ちたい
		Vector<AssetId> m_dependencies;
	};

#define EDITOR_ONLY 

	class AssetManager : public Singleton<AssetManager> {
	public:

		void loadAsync(const AssetId& id, AssetEventHandle& handle, AssetEventDelegate&& d) {

			EDITOR_ONLY ScopeLock lock(m_lock);

			// TODO Read Lock
			auto itr = m_loaders.find(id);
			if (itr == m_loaders.end())return;

			auto& loader = itr->second;

			loader->addLoadEvent(handle, std::move(d));

			auto locale = getAssetLocale(loader->getAssetType());

			

		}



		void addLoadEvent(const AssetId& id, AssetEventHandle& handle, AssetEventDelegate&& d) {

			EDITOR_ONLY ScopeLock lock(m_lock);

			// TODO Read Lock
			auto itr = m_loaders.find(id);
			if (itr == m_loaders.end())return;

			itr->second->addLoadEvent(handle, std::move(d));

		}

		void setLocale(StringView group, const Locale& locale);
		Locale getLocale(StringView group) const;
		Locale getAssetLocale(StringView group) const;

	private:

		void reload() {

		}


		struct AssetDatabase {
			AssetId id;
			String type;
			Vector<AssetId> dependencies;
		};

		void loadDatabase() {

			Vector<AssetDatabase> databases;

			for (auto& item : databases) {

				auto itr = m_factories.find(item.type);
				if (itr == m_factories.end()) {
					LOG_WARNING("アセットファクトリが見つかりません。");
					continue;
				}

				auto& factory = *itr->second;

				m_loaders[item.id] = std::make_unique<AssetLoader>(item.id, factory);

			}
		}

	private:

		// TODO 
		// * 言語変更
		//	  * 音声/言語/字幕などグループごと
		//    
		// * アセット更新
		// * アセット削除
		// * アセット更新

		struct LocaleGroup {
			Locale locale;
			Set<String> assetTypes;
		};

	private:
		EDITOR_ONLY SpinLock m_lock;
		HashMap<UUID, UPtr<AssetLoader>> m_loaders;
		Map<String, UPtr<AssetFactory>> m_factories;

		// Locale
		mutable SpinLock m_localeLock; // TODO ReadWriteLock化
		Map<String, HashSet<String>, std::less<>> m_groupMap;
		Map<String, Locale, std::less<>> m_localeMapForGroup;
		Map<String, Locale, std::less<>> m_localeMapForAssetType;
	};

}
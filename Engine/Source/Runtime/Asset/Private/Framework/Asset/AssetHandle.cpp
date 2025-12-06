//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Asset/Asset.h>
#include <Framework/Asset/AssetHandle.h>
#include <Framework/Asset/AssetManager.h>

namespace ob::asset {

	//! @brief アセットの読み込みが完了して、アセットにアクセス可能か。
	bool AssetHandleBase::isReady() const {
		return m_asset != nullptr;
	}

	//! @brief  読み込み済みのアセットIDを取得
	//! @details アセットの読み込みが完了していない場合は無効なIDを返します。
	auto AssetHandleBase::getPath() const {
		return m_asset ? m_asset->getId() : AssetId{};
	}

	//! @brief アセットの非同期読み込みをリクエストする
	//! @details onLoadイベントは通常AssetSwapジョブで呼び出されます。ただし、アセットが読み込み済みの場合は即時呼び出されます。
	//! @param id 読み込むアセット
	//! @param onLoad 読み込み完了コールバック
	void AssetHandleBase::load(AssetId id, Action onLoad) {
		unload();
		m_loaded = std::move(onLoad);
		AssetManager::Instance().addLoadEvent(id, m_hLoad, { *this,&AssetHandleBase::onLoad });
	}

	//! @brief アセットをアンロードする
	void AssetHandleBase::unload() {
		m_hLoad.remove();
		m_loaded = {};
		m_asset = {};
	}

	//! @brief デストラクタ
	AssetHandleBase::~AssetHandleBase() {
		unload();
	}

	//! @brief 読み込み完了イベント
	void AssetHandleBase::onLoad(const Ref<Asset>& asset) {

		// Swapが終わるまで寿命を延長
		Ref<Asset> old = m_asset;

		m_asset = asset;

		if (m_loaded) {
			m_loaded();
		}
	}

}
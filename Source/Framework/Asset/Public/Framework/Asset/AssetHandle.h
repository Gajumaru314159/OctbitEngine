//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Asset/AssetId.h>
#include <Framework/Asset/AssetTypes.h>

namespace ob::asset {

	class Asset;

	//! @brief アセットハンドル基底
	class AssetHandleBase {
	public:

		//! @brief アセットの読み込みが完了して、アセットにアクセス可能か。
		bool isReady() const;

		//! @brief  読み込み済みのアセットIDを取得
		//! @details アセットの読み込みが完了していない場合は無効なIDを返します。
		auto getPath() const;

		//! @brief アセットの非同期読み込みをリクエストする
		//! @details onLoadイベントは通常AssetSwapジョブで呼び出されます。ただし、アセットが読み込み済みの場合は即時呼び出されます。
		//! @param id 読み込むアセット
		//! @param onLoad 読み込み完了コールバック
		void load(AssetId id, Action onLoad);

		//! @brief アセットをアンロードする
		void unload();

	protected:

		AssetHandleBase() = default;
		virtual ~AssetHandleBase();

		void onLoad(const SPtr<Asset>& asset);

	protected:
		SPtr<Asset>         m_asset;
		Action              m_loaded;

		// NOTE EventNotifierはメモリサイズを使用するため専用のハンドルを実装しても良いかもしれない
		AssetEventHandle    m_hLoad;
	};

	//! @brief アセットハンドル
	//! @tparam T アセットタイプ
	//! @details アセットの非同期読み込みを提供ます。アセットにアクセスする場合はisReady()で読み込みが完了しているかを確認してからアクセスしてください。
	//!          onLoadイベントはアンロード時にも呼び出されます。アクセス前にisReady()で確認してください。
	template<class T>
	class AssetHandle : public AssetHandleBase {
	public:
		AssetHandle() {}
		const T* operator->() const {
			validate();
			return static_cast<const T*>(m_asset.get());
		}
		const T& operator*() const {
			validate();
			return *static_cast<const T*>(m_asset.get());
		}
	private:
		void validate() const {
			OB_ASSERT(m_asset, "アセットのハンドルが無効です。isReady()でアクセス可能か確認してからアクセスしてください。");
		}
	};

}
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Asset/AssetId.h>

namespace ob::core {
	class Locale;
}

namespace ob::asset {

	//! @brief アセット基底
	class Asset {
	public:

		//! @brief AssetId を取得
		AssetId getId() const {
			return m_id;
		}
	public:

		//! @brief アセットグループのロケールを設定する
		//! @details アセットのロケールはアセットグループごとに設定されます。
		//!          アセットグループはアセットの用途ごとに設定されています。
		//!          例えば、テキスト、フォントなどは「Subtitle」グループに設定されます。
		//! @param group ロケールグループ(例：Subtitle, Voice)
		//! @param locale ロケール
		static void SetLocale(StringView group, const Locale& locale);

		//! @brief アセットグループのロケールを取得する
		static Locale GetLocale(StringView group);

		//! @brief アセットタイプのロケールを取得する
		static Locale GetAssetLocale(StringView assetType);

	protected:
		Asset() = default;
		virtual ~Asset() = default;
	private:
		const AssetId m_id;
	};

}
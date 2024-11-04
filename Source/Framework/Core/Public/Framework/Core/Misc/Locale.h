//***********************************************************
//! @file
//! @brief		Yes/No
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Template/Container/FixedVector.h>
#include <Framework/Core/String/StringView.h>
#include <Framework/Core/String/Format.h>

namespace ob::core {

	//! @brief ロケール
	//! @details IETF言語タグを表すクラス
	//!			 * language (言語)
	//! 		 * script (スクリプト)
	//! 		 * region (地域)
	//! 		 * variant (バリアント)
	//! 		 * extension (拡張)
	//! 		 * privateuse (プライベート使用)
	//!			 で構成され、それぞれの要素はハイフンで区切られる。
	//! 		 例: ja-JP-u-ca-japanese	
	//! @link	 https://en.wikipedia.org/wiki/IETF_language_tag
	class Locale {
	public:

		Locale() = default;

		//! @brief IETFタグからロケールを生成する
		Locale(StringView tag);

		//! @brief 空か
		bool empty()const { return m_subtags.empty(); }

		//! @brief サブタグの数を取得する 
		size_t size()const { return m_subtags.size(); }

		//! @brief 対象のロケールが自身のロケールに含まれるか
		//! @details @code
		//!          Locale("en").contains(Locale("en-US")) == true
		//!          Locale("en-US").contains(Locale("en")) == false
		//!          Locale("en").contains(Locale("en")) == true
		//!          @endcode
		bool contains(const Locale& other) const;

		//! @brief 言語タグを取得する 
		StringView tag() const { return m_tag; }

		//! @brief サブタグを取得する
		StringView at(s32 index)const;

		//! @brief 言語サブタグを取得する 
		StringView language() const { return at(0); }

		//! @brief 文字体系サブタグを取得する 
		StringView script() const { return at(1); }

		//! @brief 地域サブタグを取得する 
		StringView region() const { return at(2); }

		//! @brief 異体サブタグを取得する 
		StringView variant() const { return at(3); }

		//! @brief 拡張サブタグを取得する 
		StringView extension() const { return at(4); }

		//! @brief 私用サブタグを取得する 
		StringView privateuse() const { return at(5); }

	public:

		//! @brief システム言語を取得する
		static Locale System();

	private:
		String m_tag;
		FixedVector<u8,8> m_subtags;
	};

}

//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::Locale, ob::core::Char> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}

	template<typename FormatContext>
	auto format(const ob::core::Locale& value, FormatContext& ctx) -> decltype(ctx.out()) {
		return format_to(ctx.out(), "{}", value.tag());
	}
};

//===============================================================
// ハッシュ化
//===============================================================
template<>
struct std::hash<ob::core::Locale> {
	size_t operator()(const ob::core::Locale& value)const {
		return std::hash<ob::core::StringView>()(value.tag());
	}
};

//! @endcond
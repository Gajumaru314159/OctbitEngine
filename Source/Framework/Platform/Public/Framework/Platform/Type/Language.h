//***********************************************************
//! @file
//! @brief		言語一覧
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  言語
    //@―---------------------------------------------------------------------------
    enum class Language {
        Unknown = -1,	    //!< 不明

        Japanese,	        //!< 日本語
        English,	        //!< 英語
        French,	            //!< フランス語
        Italian,	        //!< イタリア語
        German,	            //!< ドイツ語
        Spanish,	        //!< スペイン語
        Korean,	            //!< 韓国語
        Portuguese,	        //!< ポルトガル語
        Russian,	        //!< ロシア語
        Chinese,	        //!< 中国語


        /*
        Afrikaans,	        // アフリカ語
        Arabic,	            // アラビア語
        Basque,	            // バスク語
        Belarusian,	        // ベラルーシ語
        Bulgarian,	        // ブルガリア語
        Catalan,	        // カタロニア語
        Chinese,	        // 中国語
        Czech,	            // チェコ語
        Danish,	            // デンマーク語
        Dutch,	            // オランダ語
        English,	        // 英語
        Estonian,	        // エストニア語
        Faroese,	        // フェロー語
        Finnish,	        // フィンランド語
        French,	            // フランス語
        German,	            // ドイツ語
        Greek,	            // ギリシャ語
        Hebrew,	            // ヘブライ語
        Icelandic,	        // アイスランド語
        Indonesian,	        // インドネシア語
        Italian,	        // イタリア語
        Japanese,	        // 日本語
        Korean,	            // 韓国語
        Latvian,	        // ラトビア語
        Lithuanian,	        // リトアニア語
        Norwegian,	        // ノルウェー語
        Polish,	            // ポーランド語
        Portuguese,	        // ポルトガル語
        Romanian,	        // ルーマニア語
        Russian,	        // ロシア語
        SerboCroatian,	    // セルビアクロアチア語
        Slovak,	            // スロバキア語
        Slovenian,	        // スロベニア語
        Spanish,	        // スペイン語
        Swedish,	        // スウェーデン語
        Thai,	            // タイ語
        Turkish,	        // トルコ語
        Ukrainian,	        // ウクライナ語
        Vietnamese,	        // ベトナム語
        ChineseSimplified,	// 中国語簡体字
        ChineseTraditional,	// 中国語繁体字
        Hungarian,          // ハンガリー語
        */
        MAX,                // 言語数
    };

}
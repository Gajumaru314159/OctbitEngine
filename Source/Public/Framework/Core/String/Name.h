﻿//***********************************************************
//! @file
//! @brief		識別用名
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {

    namespace detail {
        class NameData;
    }

    //@―---------------------------------------------------------------------------
    //! @brief      識別用名
    //! 
    //! @details    Name クラスは高速な比較が可能です。<br>
    //!             下記の問題点を解決しています。
    //!             * Stringを直接比較：Ο(n)
    //!             * Stringのハッシュ値を比較：重複
    //!             処理のコストは以下の通りです。
    //!             * 新しい名前の構築：低速
    //!             * 辞書に存在する名前の構築：
    //!             * コピー：高速
    //!             * 比較：高速
    //@―---------------------------------------------------------------------------
    class Name {
    public:
        using Hash = u32;
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  デフォルトコンストラクタ
        //@―---------------------------------------------------------------------------
        Name();

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(文字列から)
        //@―---------------------------------------------------------------------------
        explicit Name(StringView name);

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ(ハッシュ値から)
        //! 
        //! @details    ハッシュ値の計算は内部実装に依存します。
        //@―---------------------------------------------------------------------------
        explicit Name(Hash hash);

        Name(const Name&) = default;
        Name(Name&&) = default;
        Name& operator=(const Name&) = default;
        Name& operator=(Name&&) = default;

        //@―---------------------------------------------------------------------------
        //! @brief  代入演算子(StringView)
        //@―---------------------------------------------------------------------------
        Name& operator=(StringView name);
        bool operator==(const Name&)const;
        bool operator!=(const Name&)const;

        //@―---------------------------------------------------------------------------
        //! @brief  名前をStringViewで取得
        //@―---------------------------------------------------------------------------
        StringView toSV()const;

        //@―---------------------------------------------------------------------------
        //! @brief  空か
        //@―---------------------------------------------------------------------------
        bool empty()const;

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  NameData から生成(NameDictionary用)
        //@―---------------------------------------------------------------------------
        Name(const detail::NameData& nameData);

    private:
        void setName(StringView name);
        void clear();
    private:
        StringView m_view;
        Hash m_hash;
    };



    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief  等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Name::operator==(const Name& rhs)const {
        return m_hash == rhs.m_hash;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  否等価演算子
    //@―---------------------------------------------------------------------------
    inline bool Name::operator!=(const Name& rhs)const {
        return !(*this == rhs);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  名前をStringViewで取得
    //@―---------------------------------------------------------------------------
    inline StringView Name::toSV()const {
        return m_view;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  空か
    //@―---------------------------------------------------------------------------
    inline bool Name::empty()const {
        return m_view.empty();
    }

    //! @endcond
}// namespcae ob
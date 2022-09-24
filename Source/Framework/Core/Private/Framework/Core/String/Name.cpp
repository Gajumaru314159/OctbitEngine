﻿//***********************************************************
//! @file
//! @brief		識別用名前クラス
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/String/Name.h>
#include <Framework/Core/String/NameDictionary.h>

namespace ob::core
{

    //@―---------------------------------------------------------------------------
    //! @brief  デフォルトコンストラクタ
    //@―---------------------------------------------------------------------------
    Name::Name() {
        clear();
    }

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(文字列から)
    //@―---------------------------------------------------------------------------
    Name::Name(StringView name) {
        setName(name);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ(ハッシュ値から)
    //! 
    //! @details    ハッシュ値の計算は内部実装に依存します。
    //@―---------------------------------------------------------------------------
    Name::Name(Hash hash) {
        *this = NameDictionary::Get().findName(hash);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  NameData から生成(NameDictionary用)
    //@―---------------------------------------------------------------------------
    Name::Name(const internal::NameData& nameData) 
        : m_view(nameData.getName())
        , m_hash(nameData.getHash())
    {
    }

    //@―---------------------------------------------------------------------------
    //! @brief  代入演算子(StringView)
    //@―---------------------------------------------------------------------------
    Name& Name::operator=(StringView name) {
        setName(name);
        return *this;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  名前を設定
    //@―---------------------------------------------------------------------------
    void Name::setName(StringView name) {
        if (name.empty()) {
            clear();
        } else {
            *this = std::move(NameDictionary::Get().makeName(name));
        }
    }

    //@―---------------------------------------------------------------------------
    //! @brief  クリア
    //@―---------------------------------------------------------------------------
    void Name::clear() {
        m_view = StringView();
        m_hash = 0;
    }

}// namespace ob
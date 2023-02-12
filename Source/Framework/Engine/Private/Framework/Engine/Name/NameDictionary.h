﻿//***********************************************************
//! @file
//! @brief		識別用名辞書
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/IModule.h>
#include <Framework/Engine/Name.h>

namespace ob::engine {

    //! @cond
    namespace internal{

        class NameData {
        public:
            using Hash = Name::Hash;
        public:
            NameData(StringView name, Hash hash) 
                : m_name(name)
                , m_hash(hash)
            {
            }
            StringView getName()const { return m_name; }
            Hash getHash()const { return m_hash; }
        private:
            String m_name;
            Hash m_hash;
        };

    }
    //! @endcond


    //@―---------------------------------------------------------------------------
    //! @brief  識別用名辞書
    //@―---------------------------------------------------------------------------
    class NameDictionary:public IModule {
    public:
        //@―---------------------------------------------------------------------------
        //! @brief      取得
        //@―---------------------------------------------------------------------------
        static NameDictionary& Get();
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      Nameを作成して辞書に追加
        //! 
        //! @details    文字列に対してユニークなハッシュ値を割り当てます。
        //@―---------------------------------------------------------------------------
        Name makeName(StringView name);

        //@―---------------------------------------------------------------------------
        //! @brief      ハッシュ値からNameを取得
        //! 
        //! @details    見つからない場合は空の Name を返します。
        //@―---------------------------------------------------------------------------
        Name findName(Name::Hash hash)const;

    private:

        mutable SpinLock m_lock;
        HashMap<Name::Hash, internal::NameData> m_dict;

    };

}// namespcae ob::core
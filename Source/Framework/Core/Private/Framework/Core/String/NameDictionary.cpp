//***********************************************************
//! @file
//! @brief		識別用名辞書
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/String/NameDictionary.h>
#include <Framework/Core/Misc/CRCHash.h>

namespace ob::core{
    
    //@―---------------------------------------------------------------------------
    //! @brief      Nameを作成して辞書に追加
    //! 
    //! @details    文字列に対してユニークなハッシュ値を割り当てます。
    //@―---------------------------------------------------------------------------
    Name NameDictionary::makeName(StringView nameSV) {

        if (nameSV.empty())return Name();

        // ハッシュ値計算
        auto hash = CRCHash::Compute(nameSV.data(), (s32)nameSV.size());

        // 追加済みかチェック
        // ※重複でハッシュが違うものは後で検出
        Name name = findName(hash);
        if (name.toSV() == nameSV) {
            return std::move(name);
        }

        // 追加
        ScopeLock lock(m_lock);
        
        while (true) {

            auto itr = m_dict.find(hash);

            // 新規追加
            if (itr == m_dict.end()) {
                auto& item = m_dict.emplace(hash, internal::NameData(nameSV, hash));
                return Name(item.first->second);
            }

            // 追加済み(Hashは変更済み)
            if (itr->second.getName() == nameSV) {
                return Name(itr->second);
            }

            // 同じハッシュの別名がある場合はハッシュ変更
            {
                hash++;
            }

        }

    }

    //@―---------------------------------------------------------------------------
    //! @brief      ハッシュ値からNameを取得
    //! 
    //! @details    見つからない場合は空の Name を返します。
    //@―---------------------------------------------------------------------------
    Name NameDictionary::findName(Name::Hash hash)const {
        ScopeLock lock(m_lock);
        auto itr = m_dict.find(hash);
        if (itr == m_dict.end()) {
            return Name();
        }
        return Name(itr->second);
    }

}// namespace ob::core
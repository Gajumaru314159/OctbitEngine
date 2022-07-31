//***********************************************************
//! @file
//! @brief		シングルトン
//! @author		Gajumaru
//***********************************************************
#include <Framework/Singleton/Singleton.h>
#include <unordered_map>
#include <mutex>

//@―---------------------------------------------------------------------------
//! @brief          共有インスタンスを取得
//! 
//! @param type     型タイプ
//! @param getter   スタティック・インスタンスの取得関数ポインタ
//! @param dest     インスタンスの出力先
//@―---------------------------------------------------------------------------
void GetSharedInstance(
    const std::type_index& type,
    void* (*getter)(),
    void*& dest) {

    using HolderMap = std::unordered_map<std::type_index, void*>;
    static HolderMap holders;
    static std::mutex m_mutex;

    std::lock_guard<std::mutex> lock(m_mutex);

    if (dest != nullptr)return;

    // 登録済みのインスタンスかチェック
    auto itr = holders.find(type);
    if (itr != holders.end()) {
        dest = itr->second;
        return;
    }

    dest = holders[type] = (*getter)();
}

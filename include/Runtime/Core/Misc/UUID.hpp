//***********************************************************
//! @file
//! @brief UUID
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Core/Base/Fwd.hpp>

#include <EASTL/fixed_vector.h>
#include <EASTL/hash_map.h>

namespace ob
{

    //-----------------------------------------------------------
    //! @brief UUIDクラス
    //-----------------------------------------------------------
    class UUID
    {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        UUID();                                      // デフォルトコンストラクタ


        //===============================================================
        // オペレータ
        //===============================================================
        bool operator==(const UUID& other)const;    // 等価演算子
        bool operator!=(const UUID& other)const;    // 否等価演算子


        String  ToString()const;                    // 文字列表現で取得する
        b32     IsEmpty() const;                    // 空のUUIDか判定する

        const Byte* data() const;                   //内部表現をバイト列として取得する

    public:

        static UUID Generate();                             // UUIDの生成
        static UUID FromText(const StringView& uuidText);   // UUID文字列からUUIDオブジェクトを生成
        static UUID FromByte(const Byte* bytes);            // バイト列から固有のUUIDを生成

    private:

        Byte m_data[16];//!< 内部データ

    };






    //===============================================================
    // インライン関数
    //===============================================================

    //-----------------------------------------------------------
    //! @brief 等価演算子
    //-----------------------------------------------------------
    inline bool UUID::operator==(const UUID& other) const
    {
        for (s32 i = 0; i < 16; i++)if(m_data[i] != other.m_data[i])return FALSE;
        return TRUE;
    }


    //-----------------------------------------------------------
    //! @brief 否等価演算子
    //-----------------------------------------------------------
    inline bool UUID::operator!=(const UUID& other) const
    {
        return !(operator==(other));
    }


    //-----------------------------------------------------------
    //! @brief 内部表現をバイト列として取得
    //-----------------------------------------------------------
    inline const Byte* UUID::data() const {
        return m_data;
    }


}// namespace ob

 
 
 //==============================================================================



//! @cond
// for unordered_map key
namespace eastl
{
    //! @brief ハッシュクラス
    template <> struct hash<ob::UUID>
    {
        //! @brief UUIDをハッシュ値に変換
        size_t operator()(const ob::UUID& p) const;
    };
}// namespace eastl
//! @endcond
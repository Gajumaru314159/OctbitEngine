//***********************************************************
//! @file
//! @brief UUID
//! @author Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/String/String.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief UUIDクラス
    //@―---------------------------------------------------------------------------
    struct UUID {
    public:
        u32 data[4];

    public:

        //@―---------------------------------------------------------------------------
        //! @brief ランダムなUUIDを生成
        //@―---------------------------------------------------------------------------
        static UUID Generate();

        //@―---------------------------------------------------------------------------
        //! @brief			UUID文字列からUUIDオブジェクトを生成
        //! 
        //! @details		XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXの形で表される16進数の並び
        //! @param uuidText UUIDの文字列表現
        //@―--------------------------------------------------------------------
        static Optional<UUID> FromString(const StringView& uuidText);

        //@―---------------------------------------------------------------------------
        //! @brief デフォルトコンストラクタ(ゼロ初期化)
        //@―---------------------------------------------------------------------------
        constexpr UUID();

        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ(要素指定)
        //@―---------------------------------------------------------------------------
        constexpr UUID(u32 data0,u32 data1,u32 data2,u32 data3);


        //===============================================================
        // オペレータ
        //===============================================================
        constexpr bool operator==(const UUID& other)const;  //!< 等価演算子
        constexpr bool operator!=(const UUID& other)const;  //!< 否等価演算子
        constexpr bool operator<=(const UUID& other)const;  //!< 比較演算子
        constexpr bool operator<(const UUID& other)const;   //!< 比較演算子
        constexpr bool operator>=(const UUID& other)const;  //!< 比較演算子
        constexpr bool operator>(const UUID& other)const;   //!< 比較演算子


        //@―---------------------------------------------------------------------------
        //! @brief 文字列表現で取得
        //! 
        //! @return XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXの形で表される文字列表現
        //@―---------------------------------------------------------------------------
        void  toString(Char(&dest)[37])const;


        //@―---------------------------------------------------------------------------
        //! @brief 文字列表現で取得
        //! 
        //! @return XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXXの形で表される文字列表現
        //@―---------------------------------------------------------------------------
        String  toString()const;


        //@―---------------------------------------------------------------------------
        //! @brief 空のUUIDか判定
        //! 
        //! @retval true	空
        //! @retval false	空ではない
        //@―---------------------------------------------------------------------------
        constexpr bool empty() const;

    private:

        friend class std::hash<UUID>;

        constexpr s32 compare(const UUID& b)const {
            for (s32 i = 0; i < 4; i++) {
                if (data[i] == b.data[i])continue;
                if (data[i] > b.data[i])return 1;
                if (data[i] < b.data[i])return -1;
            }
            return 0;
        }

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond
    
    //@―---------------------------------------------------------------------------
    //! @brief デフォルトコンストラクタ(ゼロ初期化)
    //@―---------------------------------------------------------------------------
    constexpr UUID::UUID():UUID(0,0,0,0) {
    }

    //@―---------------------------------------------------------------------------
    //! @brief コンストラクタ(要素指定)
    //@―---------------------------------------------------------------------------
    constexpr UUID::UUID(u32 data0, u32 data1, u32 data2, u32 data3)
        : data{data0,data1,data2,data3}
    {
    }

    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    constexpr bool UUID::operator==(const UUID& other) const { return compare(other) == 0; }
    constexpr bool UUID::operator!=(const UUID& other) const { return compare(other) != 0; }
    constexpr bool UUID::operator<(const UUID& other) const { return compare(other) < 0; }
    constexpr bool UUID::operator<=(const UUID& other) const { return compare(other) <= 0; }
    constexpr bool UUID::operator>=(const UUID& other) const { return compare(other) >= 0; }
    constexpr bool UUID::operator>(const UUID& other) const { return compare(other) > 0;}


    //@―---------------------------------------------------------------------------
    //! @brief 空のUUIDか判定
    //! 
    //! @retval true	空
    //! @retval false	空ではない
    //@―---------------------------------------------------------------------------
    constexpr bool UUID::empty() const {
        return
            data[0] == 0 &&
            data[1] == 0 &&
            data[2] == 0 &&
            data[3] == 0;
    }

    //! @endcond
}// namespace ob::core


//===============================================================
// フォーマット
//===============================================================
//! @cond
template <> struct fmt::formatter<ob::core::UUID, ob::core::Char> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template<typename FormatContext>
    auto format(ob::core::UUID value, FormatContext& ctx) -> decltype(ctx.out()) {
        ob::core::Char text[37];
        value.toString(text);
        return format_to(ctx.out(), TC("{}"), text);
    }
};

//===============================================================
// ハッシュ化
//===============================================================
template<>
struct std::hash<ob::core::UUID> {
    constexpr size_t operator()(const ob::core::UUID& value)const {

        constexpr ob::u64 offset_basis = 14695981039346656037u;
        constexpr ob::u64 fnv_prime = 1099511628211u;
        ob::u64 result = offset_basis;

        for (size_t i = 0; i < 4; ++i) {
            result ^= static_cast<ob::u64>(value.data[i]);
            result *= fnv_prime;
        }

        return result;
    }
};

//! @endcond
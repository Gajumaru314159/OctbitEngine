﻿//***********************************************************
//! @file
//! @brief		ビットフラグ
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief		enum値をビットフラグとして扱うためのラップクラス
    //! @details	enum型の内部値が1以上TBitsTypeの最大値以下の2のべき乗である必要があります。
    //! @tparam		TEnum ビットフラグに用いるenum型
    //! @tparam		BitType 内部でビットを管理する変数の型
    //@―---------------------------------------------------------------------------
    template<typename TEnum, typename TBitsType = u32>
    class bit_flags {
    public:

        //! @brief bit_flagss<TEnum, TBitsType>
        using this_type = typename bit_flags<TEnum, TBitsType>;

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        bit_flags() :m_bits(0) {}													//!< コンストラクタ
        explicit bit_flags(TEnum bit) :m_bits(static_cast<TBitsType>(bit)) {}		//!< コンストラクタ(初期値指定)
        explicit bit_flags(TBitsType flags) :m_bits(flags) {}						//!< コンストラクタ(初期値指定)
        bit_flags(const this_type& rhs) :m_bits(rhs.m_bits) {}					//!< コピーコンストラクタ


        //===============================================================
        // オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief コピー代入演算子
        //@―---------------------------------------------------------------------------
        bit_flags<TEnum>& operator=(const bit_flags<TEnum>& rhs) {
            m_bits = rhs.m_bits;
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief OR代入演算子
        //@―---------------------------------------------------------------------------
        bit_flags<TEnum>& operator|=(const bit_flags<TEnum>& rhs) {
            m_bits |= rhs.m_bits;
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief AND代入演算子
        //@―---------------------------------------------------------------------------
        bit_flags<TEnum>& operator&=(const bit_flags<TEnum>& rhs) {
            m_bits &= rhs.m_bits;
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief XOR代入演算子
        //@―---------------------------------------------------------------------------
        bit_flags<TEnum>& operator^=(const bit_flags<TEnum>& rhs) {
            m_bits ^= rhs.m_bits;
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief ビット反転演算子
        //@―---------------------------------------------------------------------------
        bit_flags<TEnum> operator~() const {
            bit_flags<TEnum> result;
            result.m_bits = (TBitsType)(~m_bits);
            return result;
        }

        //@―---------------------------------------------------------------------------
        //! @brief TBitsType変換演算子 
        //@―---------------------------------------------------------------------------
        explicit operator TBitsType() const {
            return m_bits;
        }

        //@―---------------------------------------------------------------------------
        //! @brief		TEnum変換演算子
        //!
        //! @details	ビットフラグに単一のフラグが設定されている時のみTEnum型として正しい値が返される。
        //@―---------------------------------------------------------------------------
        operator TEnum() const {
            return static_cast<TEnum>(m_bits);
        }

        //@―---------------------------------------------------------------------------
        //! @brief		フラグを保持していないことを確認する
        //@―---------------------------------------------------------------------------
        bool operator!() const {
            return !m_bits;
        }

        //@―---------------------------------------------------------------------------
        //! @brief	いずれかのフラグを保持しているかを確認する
        //@―---------------------------------------------------------------------------
        explicit operator bool() const {
            return !!m_bits;
        }

        //@―---------------------------------------------------------------------------
        //! @brief			指定されたフラグが設定されているか確認する
        //! 
        //! @param value	確認するフラグ
        //@―---------------------------------------------------------------------------
        bool has(this_type& value)const {
            return (m_bits & value.m_bits) == value.m_bits;
        }

        //@―---------------------------------------------------------------------------
        //! @brief			指定されたフラグが設定されているか確認する
        //! 
        //! @param value	確認するフラグ
        //@―---------------------------------------------------------------------------
        bool has(TEnum value)const {
            return (m_bits & static_cast<TBitsType>(value));
        }

        //@―---------------------------------------------------------------------------
        //! @brief			指定されたフラグを1に設定する
        //! 
        //! @param value	指定するフラグ
        //@―---------------------------------------------------------------------------
        this_type& on(TEnum value) {
            m_bits |= static_cast<TBitsType>(value);
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief			指定されたフラグを0に設定する
        //! @param value	指定するフラグ
        //@―---------------------------------------------------------------------------
        this_type& off(TEnum value) {
            m_bits &= ~static_cast<TBitsType>(value);
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief			指定されたフラグを0または1に設定する
        //! 
        //! @param value	指定するフラグ
        //! @param bit		設定するビット値
        //@―---------------------------------------------------------------------------
        this_type& set(TEnum value, bool bit) {
            if (bit)on(value);
            else	off(value);
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief			内部値をenum値として取得する
        //! @details		複数のフラグが設定されている場合は未定義。
        //! @return			enum値
        //@―---------------------------------------------------------------------------
        TEnum get_enum()const {
            return static_cast<TEnum>(m_bits);
        }

    private:

        TBitsType m_bits;	//!< 内部データ

    };

}// namespace ob
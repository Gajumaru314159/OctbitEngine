//***********************************************************
//! @file
//! @brief		ビットフラグ
//! @author		Gajumaru
//***********************************************************
#pragma once
//#include <Core/Base/Fwd.hpp>
#include <Core/Base/BaseType.hpp>

namespace ob
{

	//! @brief 1<<digit
#define BIT(digit)	(1<<(digit))

	//-----------------------------------------------------------
	//! @brief		enum値をビットフラグとして扱うためのラップクラス
	//! @details	enum型の内部値が1以上BitsTypeの最大値以下の2のべき乗である必要があります。
	//! @tparam		EnumType ビットフラグに用いるenum型
	//! @tparam		BitType 内部でビットを管理する変数の型
	//-----------------------------------------------------------
	template<typename EnumType, typename BitsType = u32>
	class BitFlags
	{
	public:

		//! @brief BitFlags<EnumType, BitsType>
		using this_type = typename BitFlags<EnumType, BitsType>;

	public:

		//===============================================================
		// コンストラクタ / デストラクタ
		//===============================================================
		BitFlags():m_bits(0){}													//!< コンストラクタ
		explicit BitFlags(EnumType bit):m_bits(static_cast<BitsType>(bit)){}	//!< コンストラクタ(初期値指定)
		explicit BitFlags(BitsType flags):m_bits(flags){}						//!< コンストラクタ(初期値指定)
		BitFlags(const this_type& rhs):m_bits(rhs.m_bits){}						//!< コピーコンストラクタ


		//===============================================================
		// オペレータ
		//===============================================================

		//-----------------------------------------------------------
		//! @brief コピー代入演算子
		//-----------------------------------------------------------
		BitFlags<EnumType>& operator=(const BitFlags<EnumType>& rhs)
		{
			m_bits = rhs.m_bits;
			return *this;
		}

		//-----------------------------------------------------------
		//! @brief OR代入演算子
		//-----------------------------------------------------------
		BitFlags<EnumType>& operator|=(const BitFlags<EnumType>& rhs)
		{
			m_bits |= rhs.m_bits;
			return *this;
		}

		//-----------------------------------------------------------
		//! @brief AND代入演算子
		//-----------------------------------------------------------
		BitFlags<EnumType>& operator&=(const BitFlags<EnumType>& rhs)
		{
			m_bits &= rhs.m_bits;
			return *this;
		}

		//-----------------------------------------------------------
		//! @brief XOR代入演算子
		//-----------------------------------------------------------
		BitFlags<EnumType>& operator^=(const BitFlags<EnumType>& rhs)
		{
			m_bits ^= rhs.m_bits;
			return *this;
		}

		//-----------------------------------------------------------
		//! @brief ビット反転演算子
		//-----------------------------------------------------------
		BitFlags<EnumType> operator~() const
		{
			BitFlags<EnumType> result;
			result.m_bits = (BitsType)(~m_bits);
			return result;
		}

		//-----------------------------------------------------------
		//! @brief BitsType変換演算子 
		//-----------------------------------------------------------
		explicit operator BitsType() const
		{
			return m_bits;
		}

		//-----------------------------------------------------------
		//! @brief		EnumType変換演算子
		//!
		//! @details	ビットフラグに単一のフラグが設定されている時のみEnumType型として正しい値が返される。
		//-----------------------------------------------------------
		operator EnumType() const
		{
			return static_cast<EnumType>(m_bits);
		}

		//-----------------------------------------------------------
		//! @brief		フラグを保持していないことを確認する
		//-----------------------------------------------------------
		bool operator!() const
		{
			return !m_bits;
		}

		//-----------------------------------------------------------
		//! @brief	いずれかのフラグを保持しているかを確認する
		//-----------------------------------------------------------
		explicit operator bool() const
		{
			return !!m_bits;
		}

		//-----------------------------------------------------------
		//! @brief			指定されたフラグが設定されているか確認する
		//! 
		//! @param value	確認するフラグ
		//-----------------------------------------------------------
		bool HasFlags(this_type& value)const
		{
			return (m_bits & value.m_bits) == value.m_bits;
		}

		//-----------------------------------------------------------
		//! @brief			指定されたフラグが設定されているか確認する
		//! 
		//! @param value	確認するフラグ
		//-----------------------------------------------------------
		bool HasFlag(EnumType value)const
		{
			return (m_bits & static_cast<BitsType>(value));
		}

		//-----------------------------------------------------------
		//! @brief			指定されたフラグを1に設定する
		//! 
		//! @param value	指定するフラグ
		//-----------------------------------------------------------
		this_type& Set(EnumType value)
		{
			m_bits |= static_cast<BitsType>(value);
			return *this;
		}

		//-----------------------------------------------------------
		//! @brief			指定されたフラグを0または1に設定する
		//! 
		//! @param value	指定するフラグ
		//! @param bit		設定するビット値
		//-----------------------------------------------------------
		this_type& Set(EnumType value, bool bit)
		{
			if (bit)Set(value);
			else	Unset(value);
			return *this;
		}

		//-----------------------------------------------------------
		//! @brief			指定されたフラグを0に設定する
		//! @param value	指定するフラグ
		//-----------------------------------------------------------
		this_type& Unset(EnumType value)
		{
			m_bits &= ~static_cast<BitsType>(value);
			return *this;
		}

		//-----------------------------------------------------------
		//! @brief			内部値をenum値として取得する
		//! @details		複数のフラグが設定されている場合は未定義。
		//! @return			enum値
		//-----------------------------------------------------------
		EnumType GetEnum()const
		{
			return static_cast<EnumType>(m_bits);
		}

	private:

		BitsType m_bits;	//!< 内部データ

	};

}// namespace ob
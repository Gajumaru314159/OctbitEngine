//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/Duration.h>
/*
namespace ob::input {

	//@―---------------------------------------------------------------------------
	//! @brief		入力インターフェイス
	//! 
	//! @details	このインターフェイスは「Aキー」や「左クリック」など、1入力ごとに割り当てられます。
	//@―---------------------------------------------------------------------------
	class IInput{
	public:
		virtual f32 value()=0;						//!< 入力値を取得
		virtual bool down()const = 0;				//!< 押された瞬間
		virtual bool up()const = 0;					//!< 離された瞬間
		virtual bool pressed()const = 0;			//!< 押している間
		virtual Duration pressedSpan()const = 0;	//!< 押されている時間
		virtual Duration releasedSpan()const = 0;	//!< 離されている時間
	};


	class InputGroup {
	public:

		template <class ...Args>
		InputGroup(const Args&... args)
		{
			//append(args...);
		}


		InputGroup operator | (const Input& lhs)const;
		InputGroup operator | (const InputGroup& lhs)const;

		constexpr bool operator ==(const Input& rhs) noexcept;
		constexpr bool operator !=(const Input& rhs) noexcept;

		s32 pressedCount()const;
		auto inputs()const->const Array<Input>&;

		bool down()const;		// 押された瞬間
		bool up()const;			// 離された瞬間
		bool pressed()const;	// 押している間
		bool released()const;	// 離している間

		Duration pressedSpan()const;	// 押されている時間
		Duration releasedSpan()const;	// 離されている時間

	private:

		void append(const Input& other);
		void append(const InputGroup& other);

	private:
		StaticArray<Input, 4> m_inputs;
	};


	class Input {
	public:
		using code_type = u8;
	public:

		Input(const IInput& rInput,code_type code, u8 userIndex = 0)noexcept;


		InputGroup operator | (const Input& lhs)const;
		InputGroup operator | (const InputGroup& lhs)const;

		constexpr bool operator ==(const Input& rhs) noexcept;
		constexpr bool operator !=(const Input& rhs) noexcept;


		constexpr InputDeviceType getDeviceType()const noexcept;
		constexpr u8 getCode()const noexcept;
		constexpr u8 getUserIndex()const noexcept;

		bool down()const;		// 押された瞬間
		bool up()const;			// 離された瞬間
		bool pressed()const;	// 押している間
		bool released()const;	// 離している間

		Duration pressedSpan()const;	// 押されている時間
		Duration releasedSpan()const;	// 離されている時間

	private:

		IInput& m_input;
		code_type m_code;
		u8 m_userIndex;

	};


	inline constexpr Input KeyPageUp{ InputDeviceType::Keyboard, 0x21 };

}// namespcae ob::input


namespace ob::input {

	inline constexpr Input::Input(const IInput& rInput,const code_type code, const u8 userIndex) noexcept
		: m_deviceType{ deviceType }
		, m_code{ code }
		, m_userIndex{ userIndex } {}


	constexpr bool Input::operator ==(const Input& rhs) noexcept {
		return
			m_deviceType == rhs.m_deviceType &&
			m_code == rhs.m_code &&
			m_userIndex == rhs.m_userIndex;
	}

	constexpr bool Input::operator !=(const Input& rhs) noexcept {
		return !(*this == rhs);
	}

	inline constexpr InputDeviceType Input::getDeviceType() const noexcept
	{
		return m_deviceType;
	}

	inline constexpr Input::code_type Input::getCode() const noexcept
	{
		return m_code;
	}

	inline constexpr u8 Input::getUserIndex() const noexcept
	{
		return m_userIndex;
	}


}*/
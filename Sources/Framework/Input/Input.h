//***********************************************************
//! @file
//! @brief		入力系のタイプ宣言
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/Duration.h>
#include <Framework/Input/InputType.h>
#include <Framework/Input/Interface/IInputDevice.h>
#include <Framework/Core/Hash/crc32.h>

namespace ob::input {

	//@―---------------------------------------------------------------------------
	//! @brief  入力オブジェクト
	//@―---------------------------------------------------------------------------
	class Input {
	public:

		Input() = default;

		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ
		//! @param deviceID	デバイスタイプ
		//! @param code		コード
		//! @param user		ユーザ番号
		//@―---------------------------------------------------------------------------
		constexpr Input(u32 deviceID, u32 code, u32 user = 0) :m_deviceId(deviceID),m_code(code), m_user(user) {}

		//@―---------------------------------------------------------------------------
		//! @berif	コンストラクタ
		//@―---------------------------------------------------------------------------
		template<typename T>
		constexpr Input(u32 type, T code, u32 user = 0) : Input(type, enum_cast(code), user) {}

		//@―---------------------------------------------------------------------------
		//! @berif	デバイスタイプを取得
		//@―---------------------------------------------------------------------------
		constexpr u32 deviceId()const noexcept { return m_deviceId; }

		//@―---------------------------------------------------------------------------
		//! @berif	
		//@―---------------------------------------------------------------------------
		constexpr u32 code()const noexcept { return m_code; }

		//@―---------------------------------------------------------------------------
		//! @berif	ユーザ番号を取得
		//@―---------------------------------------------------------------------------
		constexpr u32 user()const noexcept { return m_user; }

		//@―---------------------------------------------------------------------------
		//! @berif	状態が state か
		//@―---------------------------------------------------------------------------
		bool is(InputState state)const;

		//@―---------------------------------------------------------------------------
		//! @berif	押されているか
		//@―---------------------------------------------------------------------------
		bool down()const;

		//@―---------------------------------------------------------------------------
		//! @berif	離されているか
		//@―---------------------------------------------------------------------------
		bool up()const;

		//@―---------------------------------------------------------------------------
		//! @berif	押された瞬間か
		//@―---------------------------------------------------------------------------
		bool pressed()const;

		//@―---------------------------------------------------------------------------
		//! @berif	離された瞬間か
		//@―---------------------------------------------------------------------------
		bool released()const;

		f32  value() const;
		Vec2 xy() const;
		Vec3 xyz() const;
		/*
		//@―---------------------------------------------------------------------------
		//! @berif	バインド
		//@―---------------------------------------------------------------------------
		bool bindDown(InputHandle& handle, Delegate<void()> func)const;
		bool bindUp(InputHandle& handle, Delegate<void()> func)const;
		bool bindPressed(InputHandle& handle, Delegate<void()> func)const;
		bool bindReleased(InputHandle& handle, Delegate<void()> func)const;


		bool bind(InputState state,InputHandle& handle, Delegate<void()> func)const;
		bool bind(InputHandle& handle, Delegate<void(f32)> func)const;
		bool bind(InputHandle& handle, Delegate<void(Vec2)> func)const;
		bool bind(InputHandle& handle, Delegate<void(Vec3)> func)const;

		*/
	private:

		bool bindImpl(InputHandle&, const InputDelegate&)const;

	private:
		u32 m_deviceId;
		u32 m_user;
		u32 m_code;
	};

}


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
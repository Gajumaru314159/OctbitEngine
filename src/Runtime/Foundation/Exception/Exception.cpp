//***********************************************************
//! @file
//! @brief		例外クラス
//! @author		Gajumaru
//***********************************************************
#include "Exception.h"


namespace ob
{

	//@―---------------------------------------------------------------------------
	Exception::Exception()
		:Exception(TEXT(""))
	{

	}

	Exception::Exception(const String& message)
		:m_level(ExceptionLevel::Error)
		, m_message(message)
	{

	}

	Exception::~Exception() {

	}

	const String& Exception::GetMessage()const {
		return m_message;
	}

	void Exception::SetMessage(const Char* format, va_list args) {

	}

}// namespace ob
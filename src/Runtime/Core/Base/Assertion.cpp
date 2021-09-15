//***********************************************************
//! @file
//! @brief		アサーション
//! @author		Gajumaru
//***********************************************************
#include <Core/Base/Assertion.hpp>

namespace ob
{
	//============================================
	// エラーチェック
	//============================================

	namespace detail
	{

		void NotifyAbort(const Char* file, int line, const Char* message)
		{
			detail::NotifyException<Exception>(ExceptionLevel::Fatal, file, line, message);
		}

	}// namespace detail


	//==============================================================================
	// 例外
	//==============================================================================

	Exception::Exception()
		: m_level(ExceptionLevel::Fatal)
		, m_sourceFilePath{}
		, m_sourceFileLine(0)
		, m_assertionMessage{}
		, m_caption()
		, m_message()
	{}

	Exception::Exception(const Char* message) :Exception()
	{
		SetMessage(message);
	}




	Exception::~Exception() {}




	void Exception::SetSourceLocationInfo(ExceptionLevel level, const Char* filePath, int fileLine, const Char* assertionMessage)
	{
		m_level = level;
		m_sourceFilePath = filePath;
		m_sourceFileLine = fileLine;
		if (assertionMessage)
		{
			m_assertionMessage = assertionMessage;
		}
		String s(filePath);
		s += TC("AAA");
		m_sourceFilePath = s;
	}




	//! @brief 共通のエラーハンドラ
	static Exception::NotificationHandler	g_notifyVerificationHandler = nullptr;

	void Exception::SetNotificationHandler(Exception::NotificationHandler handler)
	{
		g_notifyVerificationHandler = handler;
	}




	Exception::NotificationHandler Exception::GetNotificationHandler()
	{
		return g_notifyVerificationHandler;
	}




	void Exception::SetMessage(const Char* format, va_list args)
	{

		// フォーマット
		static const s32 BUFFER_SIZE = 512;
		//char buf[BUFFER_SIZE];

		// TODO 各Charでのvsprintfを実装

		//s32 len = StringHelper::vsprintf(buf, BUFFER_SIZE, format, args);

		// char to Char
		//Char ucharBuf[BUFFER_SIZE];
		//StringHelper::SafeCharConvert(buf, ucharBuf, BUFFER_SIZE);
		//AppendMessage(ucharBuf, len);

		// キャプションで初期化
		m_message = GetCaption();
	}




	//! Exceptionの共通コンストラクタ用マクロ
#define OB_EXCEPTION_CONSTRUCTOR_IMPLEMENT(className) \
	className::className(const Char* message) \
	{ \
		SetMessage(message); \
	}


//==============================================================================
// LogicException
//==============================================================================

	LogicException::LogicException()
	{
		SetCaption(TC("ln::LogicException"));
	}

	OB_EXCEPTION_CONSTRUCTOR_IMPLEMENT(LogicException);



	//==============================================================================
	// RuntimeException
	//==============================================================================

	RuntimeException::RuntimeException()
	{
		SetCaption(TC("ln::RuntimeException"));
	}

	OB_EXCEPTION_CONSTRUCTOR_IMPLEMENT(RuntimeException);



	//==============================================================================
	// FatalException
	//==============================================================================

	FatalException::FatalException()
	{
		SetCaption(TC("ln::FatalException"));
	}

	OB_EXCEPTION_CONSTRUCTOR_IMPLEMENT(FatalException);



	//==============================================================================
	// NotImplementedException
	//==============================================================================

	NotImplementedException::NotImplementedException()
	{
		SetCaption(TC("ln::NotImplementedException"));
	}

	OB_EXCEPTION_CONSTRUCTOR_IMPLEMENT(NotImplementedException);



	//==============================================================================
	// IOException
	//==============================================================================

	IOException::IOException()
	{
		SetCaption(TC("ln::IOException"));
	}

	OB_EXCEPTION_CONSTRUCTOR_IMPLEMENT(IOException);



} // namespace ob
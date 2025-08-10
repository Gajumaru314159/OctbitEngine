//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Exception/Exception.h>

namespace ob::core {

    //! @brief	コンストラクタ
    Exception::Exception()
        :Exception("") {

    }


    //! @brief コンストラクタ(メッセージ指定)
    Exception::Exception(StringView message)
        :m_level(ExceptionLevel::Error)
        , m_message(message) {

    }


    //! @brief デストラクタ
    Exception::~Exception() = default;

}
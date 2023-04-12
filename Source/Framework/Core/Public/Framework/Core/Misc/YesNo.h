//***********************************************************
//! @file
//! @brief		Yes/No
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  Yes/No表現
    //! 
    //! @details    bool値の引数に意味を持たせるために使用します。
    //! ```
    //! DEFINE_YES_NO(UseDepth);
    //! DEFINE_YES_NO(UseColor);
    //! 
    //! void func(UseDepth,UseColor);
    //! void main(){
    //!     func(UseDepth::Yes,UseColor::No);
    //! }
    //! ```
    //@―---------------------------------------------------------------------------
    template <class Tag>
    class YesNo {
    public:

        //! @cond
        struct Helper {
            bool yesNo;
        };
        //! @endcond

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        explicit constexpr YesNo(bool yesNo) noexcept
            : m_yesNo{ yesNo } {
        }

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        constexpr YesNo(Helper helper) noexcept
            : m_yesNo(helper.yesNo) {
        }

        //@―---------------------------------------------------------------------------
        //! @brief  bool変換演算子
        //@―---------------------------------------------------------------------------
        explicit constexpr operator bool() const noexcept {
            return m_yesNo;
        }


    public:

        static constexpr Helper Yes{ true };    //!< Yesを表す定数
        static constexpr Helper No{ false };    //!< Yesを表す定数

    private:

        bool m_yesNo;   // 内部表現

    };


    //@―---------------------------------------------------------------------------
    //! @brief  YesNo定義マクロ
    //@―---------------------------------------------------------------------------
#define DEFINE_YES_NO(name) using name = YesNo<struct name##_tag>;


}
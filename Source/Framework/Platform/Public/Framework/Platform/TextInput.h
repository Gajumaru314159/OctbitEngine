//***********************************************************
//! @file
//! @brief		テキスト入力
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::platform {


    enum class TextInputMode {
        DenyCtrl = get_bit(0),
        AllowEnter = get_bit(1),
        AllowTab = get_bit(2),
        AllowBackSpace = get_bit(3),
        AllowDelete = get_bit(4),

        Default = AllowEnter | AllowTab | AllowBackSpace | AllowDelete,
    };

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class TextInput {
    public:

        static String GetText();
        static size_t UpdateText(StringView text, size_t cursor, TextInputMode mode = TextInputMode::Default);
        static size_t UpdateText(StringView text, TextInputMode mode = TextInputMode::Default);
        static String GetEditingText();


        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------

    private:



    };

}
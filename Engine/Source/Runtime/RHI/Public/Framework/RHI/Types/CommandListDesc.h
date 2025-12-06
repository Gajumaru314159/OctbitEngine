//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi {

#pragma region Enum

    //! @brief  コマンドリスト・タイプ
    enum class CommandListType :u32 {
        Graphic,    //!< グラフィック
        Compute,    //!< コンピュート
        Copy
        // VideoDecode
        // VideoProcess
        // VideoEncode
    };

#pragma endregion

    //! @brief  コマンドリスト定義
    struct CommandListDesc{
        String			name;	//!< 名前
        CommandListType type;   //!< 種類
    };

}
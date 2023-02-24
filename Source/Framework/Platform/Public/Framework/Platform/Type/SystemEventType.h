//***********************************************************
//! @file
//! @brief		システム・イベント型
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウ・イベント・タイプ
    //@―---------------------------------------------------------------------------
    enum class WindowEventType {
        Unknown,    //!< 不明
        Move,       //!< ウィンドウが移動
        Size,       //!< サイズ変更
        Activate,   //!< アクティブ状態に変更
        Deactivate, //!< 非アクティブ状態に変更
        Focus,      //!< フォーカスを取得
        Close,      //!< 終了時
        Destroy,    //!< 破棄
        Minimize,   //!< 最小化
        Maximize,   //!< 最大化
        DragEnter,  //!< ドラッグアイテムがウィンドウ内に入った
        DragDrop,   //!< ドラッグアイテムがドロップされた
    };

    class Draggable;

    struct WindowEventArgs {

        WindowEventType type;

        // Move
        union {
            Vec2 oldPos;
            Vec2 newPos;
        };

        // Size
        union {
            Vec2 oldSize;
            Vec2 newSize;
        };

        // Drag
        union {
            Draggable* item;
        };
    };



    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウ・イベント
    //@―---------------------------------------------------------------------------
    //! @{
    using WindowEventNotifier = EventNotifier<const WindowEventArgs&>;
    using WindowEventHandle = typename WindowEventNotifier::Handle;
    //! @}

}// namespace ob::platform
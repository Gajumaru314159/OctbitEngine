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

        Minimize,   //!< 最小化
        Maximize,   //!< 最大化

        Activate,   //!< アクティブ状態に変更
        Deactivate, //!< 非アクティブ状態に変更

        Enable,     //!< 入力有効化
        Disable,    //!< 入力無効化

        Show,       //!< 表示
        Hide,       //!< 非表示

        DragEnter,  //!< ドラッグアイテムがウィンドウ内に入った
        DragDrop,   //!< ドラッグアイテムがドロップされた

        Close,      //!< 終了時
        Destroy,    //!< 破棄
    };

    class Draggable;

    struct WindowEventArgs {

        WindowEventType type;

        // Move
        Vec2 oldPos;
        Vec2 newPos;

        // Size
        bool isSizing;
        Vec2 oldSize;
        Vec2 newSize;

        // Drag
        Draggable* item=nullptr;
    };



    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウ・イベント
    //@―---------------------------------------------------------------------------
    //! @{
    using WindowEventNotifier = EventNotifier<const WindowEventArgs&>;
    using WindowEventHandle = typename WindowEventNotifier::Handle;
    //! @}

}
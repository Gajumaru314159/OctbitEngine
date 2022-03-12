﻿//***********************************************************
//! @file
//! @brief		ウィンドウ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Type/Point.h>
#include <Framework/Core/Type/Size.h>
#include <Framework/Platform/Type/SystemEventType.h>
#include <Framework/Platform/Type/CursorType.h>
#include <Framework/Platform/Type/WindowCreationParams.h>
#include <Framework/Platform/Window/WindowState.h>


//===============================================================
// 前方宣言
//===============================================================
namespace ob::platform {
    class WindowImpl;
    class WindowNativeAccessor;
}


//===============================================================
// クラス宣言
//===============================================================
namespace ob::platform {


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウ
    //@―---------------------------------------------------------------------------
    class Window :public RefObject {
        friend class WindowNativeAccessor;
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //! 
        //! @details    生成情報を指定してウィンドウを生成する。
        //@―---------------------------------------------------------------------------
        Window(const WindowCreationDesc& desc);

        //@―---------------------------------------------------------------------------
        //! @brief      デストラクタ
        //@―---------------------------------------------------------------------------
        ~Window();


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウを表示する
        //@―---------------------------------------------------------------------------
        void show();


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウを閉じる
        //@―---------------------------------------------------------------------------
        void close();


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウ・イベントのリスナを追加する
        //@―---------------------------------------------------------------------------
        void addEventListener(WindowEventType type, const WindowEvent& event);


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウのタイトルを設定する
        //@―---------------------------------------------------------------------------
        void setTitle(StringView title);


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウのタイトルを設定する
        //@―---------------------------------------------------------------------------
        const String& title()const;

        /*

        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウの状態を取得する
        //@―---------------------------------------------------------------------------
        const WindowState& GetState()const;


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウのスタイルを取得する
        //@―---------------------------------------------------------------------------
        inline WindowStyle& GetStyle()const;


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウのスタイルを設定する
        //@―---------------------------------------------------------------------------
        void SetStyle(WindowStyle style);


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウの位置を設定する
        //@―---------------------------------------------------------------------------
        void SetPosition(Point position);


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウの位置を取得する
        //@―---------------------------------------------------------------------------
        Point GetPosition()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウを現在のモニタの中央に移動する
        //@―---------------------------------------------------------------------------
        void MoveToCenter();


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウを最大化する
        //@―---------------------------------------------------------------------------
        void Maximize();


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウを最小化する
        //@―---------------------------------------------------------------------------
        void Minimize();


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウサイズを元のサイズに戻す
        //@―---------------------------------------------------------------------------
        void RestoreSize();


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウサイズの取得
        //@―---------------------------------------------------------------------------
        Size size()const;


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウサイズの取得
        //@―---------------------------------------------------------------------------
        Size SetFullscreen(bool isFullscreen)const;

        */

        //@―---------------------------------------------------------------------------
        //! @brief              スクリーン座標の取得
        //! 
        //! @details            クライアント座標をスクリーン座標に変換して取得する。
        //! @patam clientPoint  クライアント座標
        //! @return             スクリーン座標
        //@―---------------------------------------------------------------------------
        Point getScreenPoint(const Point& clientPoint)const;


        //@―---------------------------------------------------------------------------
        //! @brief              クライアント座標の取得
        //! 
        //! @details            スクリーン座標をクライアント座標に変換して取得する。
        //! @patam screenPoint  スクリーン座標  
        //! @return             クライアント座標
        //@―---------------------------------------------------------------------------
        Point getClientPoint(const Point& screenPoint)const;


        //@―---------------------------------------------------------------------------
        //! @brief              妥当な状態か
        //! 
        //! @details            システムからウィンドウのクローズ処理が呼ばれた場合false
        //!                     を返すようになります。
        //@―---------------------------------------------------------------------------
        bool isValid()const noexcept;

    private:

        std::unique_ptr<WindowImpl> m_impl;

    };

}// namespace ob::platform
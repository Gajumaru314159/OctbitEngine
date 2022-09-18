﻿//***********************************************************
//! @file
//! @brief		ウィンドウ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/Type/SystemEventType.h>
#include <Framework/Platform/Type/CursorType.h>
#include <Framework/Platform/Type/WindowDesc.h>
#include <Framework/Platform/Type/WindowState.h>

//===============================================================
// クラス宣言
//===============================================================
namespace ob::platform {


    //@―---------------------------------------------------------------------------
    //! @brief  ウィンドウ
    //@―---------------------------------------------------------------------------
    class Window {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      デフォルトコンストラクタ
        //@―---------------------------------------------------------------------------
        Window() = default;

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //! 
        //! @details    生成情報を指定してウィンドウを生成する。
        //@―---------------------------------------------------------------------------
        Window(const WindowDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief      デストラクタ
        //@―---------------------------------------------------------------------------
        ~Window();


        //===============================================================
        // オペレータ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief              妥当な状態か
        //! 
        //! @details            システムからウィンドウのクローズ処理が呼ばれた場合false
        //!                     を返すようになります。
        //@―---------------------------------------------------------------------------
        operator bool()const;


        //===============================================================
        // コピー
        //===============================================================

        //! @cond  
        Window(const Window&) = default;
        Window(Window&&) = default;
        Window& operator = (const Window&) = default;
        Window& operator = (Window&&) = default;
        //! @endcond


        //===============================================================
        // ウィンドウ操作
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウを表示する
        //@―---------------------------------------------------------------------------
        void show();


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウを閉じる
        //@―---------------------------------------------------------------------------
        void close();


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウを最大化する
        //@―---------------------------------------------------------------------------
        void maximize();


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウを最小化する
        //@―---------------------------------------------------------------------------
        void minimize();


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウを現在のモニタの中央に移動する
        //@―---------------------------------------------------------------------------
        void moveToCenter();
        

        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウサイズを元のサイズに戻す
        //@―---------------------------------------------------------------------------
        void restoreSize();


        //===============================================================
        // 情報取得
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief              妥当な状態か
        //! 
        //! @details            システムからウィンドウのクローズ処理が呼ばれた場合false
        //!                     を返すようになります。
        //@―---------------------------------------------------------------------------
        bool isValid()const noexcept;

        
        //@―---------------------------------------------------------------------------
        //! @brief              メインウィンドウか
        //@―---------------------------------------------------------------------------
        bool isMainWindow()const;


        //@―---------------------------------------------------------------------------
        //! @brief              ウィンドウの状態を取得する
        //@―---------------------------------------------------------------------------
        WindowStates getState()const;


        //@―---------------------------------------------------------------------------
        //! @brief              スクリーン座標を取得
        //! 
        //! @details            クライアント座標をスクリーン座標に変換して取得する。
        //! @param clientPoint  クライアント座標
        //! @return             スクリーン座標
        //@―---------------------------------------------------------------------------
        Point getScreenPoint(const Point& clientPoint)const;


        //@―---------------------------------------------------------------------------
        //! @brief              クライアント座標を取得
        //! 
        //! @details            スクリーン座標をクライアント座標に変換して取得する。
        //! @param screenPoint  スクリーン座標  
        //! @return             クライアント座標
        //@―---------------------------------------------------------------------------
        Point getClientPoint(const Point& screenPoint)const;


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウのタイトルを設定する
        //@―---------------------------------------------------------------------------
        void setTitle(StringView getTitle);


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウのタイトルを設定する
        //@―---------------------------------------------------------------------------
        const String& getTitle()const;


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウの位置を設定する
        //@―---------------------------------------------------------------------------
        void setPosition(Point position);


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウの位置を取得する
        //@―---------------------------------------------------------------------------
        Point getPosition()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウのサイズを設定する
        //@―---------------------------------------------------------------------------
        void setSize(Size size);


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウサイズを取得
        //@―---------------------------------------------------------------------------
        Size getSize()const;


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウ・モードを取得する
        //@―---------------------------------------------------------------------------
        WindowMode getMode()const;


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウ・モードを設定する
        //@―---------------------------------------------------------------------------
        void setMode(WindowMode mode);


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウのスタイルを取得する
        //@―---------------------------------------------------------------------------
        WindowStyle getStyle()const;


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウのスタイルを設定する
        //@―---------------------------------------------------------------------------
        void setStyle(WindowStyle style);


        //@―---------------------------------------------------------------------------
        //! @brief      ハンドル取得
        //! 
        //!             |Platform|Type  |
        //!             |--------|------|
        //!             |Windows | HWND |
        //@―---------------------------------------------------------------------------
        void* getHandle()const;


        //===============================================================
        // イベント
        //===============================================================


        //@―---------------------------------------------------------------------------
        //! @brief      ウィンドウ・イベントのリスナを追加する
        //@―---------------------------------------------------------------------------
        void addEventListener(WindowEventType type, const WindowEvent& e);


    public:

        //@―---------------------------------------------------------------------------
        //! @brief      メインウィンドウを取得
        //@―---------------------------------------------------------------------------
        static Window getMainWindow();


    public:

        SPtr<class WindowImpl> m_impl;

    };

}// namespace ob::platform
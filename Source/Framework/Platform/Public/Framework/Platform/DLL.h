﻿//***********************************************************
//! @file
//! @brief		動的ライブラリ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/File/Path.h>

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief      動的ライブラリ
    //! 
    //! @details    DLLから生成されたインスタンスはDLLが解放されるまで
    //!             に解放される必要があります。
    //@―---------------------------------------------------------------------------
    class DLL {

        //@―---------------------------------------------------------------------------
        //! @brief      関数オブジェクト
        //! 
        //! @deiltas    内部に動的ライブラリ内の関数ポインタを保持します。
        //!             関数のシグネチャはチェックすることができないので注意してください。
        //@―---------------------------------------------------------------------------
        class Function {
            friend class DLL;
        public:

            //@―---------------------------------------------------------------------------
            //! @brief      コンストラクタ(空オブジェクト)
            //@―---------------------------------------------------------------------------
            Function() = default;

            //@―---------------------------------------------------------------------------
            //! @brief      有効な関数ポインタを持っているか
            //@―---------------------------------------------------------------------------
            operator bool()const {
                return m_ptr != nullptr;
            }

            //@―---------------------------------------------------------------------------
            //! @brief      関数呼び出し
            //!     
            //! @details    戻り値が必要な場合は以下のように指定します
            //!             ```cpp
            //!             // int型の戻り値を受け取る
            //!             int result = func.call<int>();
            //!             ```
            //!             暗黙的変換を利用できないので、必要であれば引数の型を指定してください。
            //!             ```cpp
            //!             // int型を渡すが関数のシグネチャはfloatを使用する
            //!             func.call<void,float>(12);
            //!             ```
            //@―---------------------------------------------------------------------------
            template<typename Ret = void,typename... Args>
            Ret call(Args... args)const {
                OB_CHECK_ASSERT(m_ptr, "関数オブジェクトが空です。");
                typedef Ret (*Func)(Args...);
                Func func = reinterpret_cast<Func>(m_ptr);
                return func(std::forward(args)...);
            }

        private:
            const void* m_ptr=nullptr;
        };

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //! 
        //! @details    ファイルパスを使用する場合は拡張子を除いたパスて使用します。
        //!             読み込み時にプラットフォームに応じて拡張子が追加されます。
        //!             |Windows    |Linux      |MacOS      |
        //!             |-----------|-----------|-----------|
        //!             |.dll       |.so        |.dylib     |
        //! @param path 動的ライブラリのファイル名
        //@―---------------------------------------------------------------------------
        DLL(const Path& path);

        //@―---------------------------------------------------------------------------
        //! @brief      デストラクタ
        //@―---------------------------------------------------------------------------
        ~DLL();

        //@―---------------------------------------------------------------------------
        //! @brief      関数オブジェクトを取得
        //! 
        //! @details    動的ライブラリから関数名を指定して関数オブジェクトを取得します。
        //! @param name 関数名
        //@―---------------------------------------------------------------------------
        Function getFunction(StringView name);

        //@―---------------------------------------------------------------------------
        //! @brief      関数実行
        //! 
        //! @details    戻り値のない関数を名前を指定して実行します。
        //!             呼び出すたびに関数の検索がされるので、何度も使用する場合は
        //!             getFunction() を使用して関数オブジェクトを取得してください。
        //! @param name 関数名
        //! @param ...args 引数
        //! @retval true 成功
        //! @retval false 失敗
        //@―---------------------------------------------------------------------------
        template<typename... Args>
        bool tryCall(StringView name, Args&&... args) {
            if (auto func = getFunction(name)) {
                func.call<void(Args...)>(std::forward(args)...);
                return true;
            }
            return false;
        }

        //@―---------------------------------------------------------------------------
        //! @brief      読み込み中の動的ライブラリのパスを取得
        //! 
        //! @detilas    拡張子付きのパスを返します。読み込み失敗している場合は空のパスを返します。
        //@―---------------------------------------------------------------------------
        const Path& getPath()const;

    private:

        Path m_path;
        
        // WindowsではHMODULE
        const void* m_handle;

    };

}// namespcae ob
//***********************************************************
//! @file
//! @brief		シングルトン・パターン
//! @author		Gajumaru
//***********************************************************
#pragma once

#if defined(OB_SINGLETON_BUILD)
#define OB_FND_API DLL_EXPORT
#else
#define OB_FND_API DLL_IMPORT
#endif
//@―---------------------------------------------------------------------------
//! @def		OB_FND_API
//! @brief		DLLエクスポート/インポートマクロ
//! @details	DLL出力 / DLL入力 / その他(LIB)
//@―---------------------------------------------------------------------------

extern "C"
{
    //@―---------------------------------------------------------------------------
    //! @brief          共有インスタンスを取得
    //! 
    //! @param type     型タイプ
    //! @param getter   スタティック・インスタンスの取得関数ポインタ
    //! @param dest     インスタンスの出力先
    //@―---------------------------------------------------------------------------
    OB_FND_API void GetSharedInstance(
        const std::type_index& type,
        void* (*getter)(),
        void*& dest);
}

namespace ob::foundation {

    //@―---------------------------------------------------------------------------
    //! @brief          シングルトン・パターン
    //! 
    //! @details        DLL間で共通のインスタンスを生成するため、インスタンスは
    //!                 別DLLで管理される。
    //@―---------------------------------------------------------------------------
    template<class T>
    class Singleton {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      インスタンスを取得
        //@―---------------------------------------------------------------------------
        static T& Instance()noexcept;

    protected:

        Singleton() = default;
        virtual ~Singleton()=default;

    private:

        //! @cond  
        Singleton(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton& operator = (const Singleton&) = delete;
        Singleton& operator = (Singleton&&) = delete;
        //! @endcond  


        //@―---------------------------------------------------------------------------
        //! @brief      スタティック・インスタンスを取得
        //@―---------------------------------------------------------------------------
        static void* GetStaticInstance();

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond


    //@―---------------------------------------------------------------------------
    //! @brief      インスタンスを取得
    //@―---------------------------------------------------------------------------
    template<class T>
    inline T& Singleton<T>::Instance()noexcept {
        // 
        static void* pInstance = nullptr;
        if (pInstance == nullptr) {
            // 各ライブラリで初回の取得のみ呼ばれる
            // 管理DLLにインスタンスが登録されていない場合は GetStaticInstance からインスタンスを取得し、
            // 登録されている場合は pInstance にインスタンスを返す。
            GetSharedInstance(typeid(T), &GetStaticInstance, pInstance);
        }
        return *reinterpret_cast<T*>(pInstance);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      スタティック・インスタンスを取得
    //@―---------------------------------------------------------------------------
    template<class T>
    inline void* Singleton<T>::GetStaticInstance() {
        static T t;
        return reinterpret_cast<void*>(&t);
    }

    //! @endcond
}// namespcae ob::foundation
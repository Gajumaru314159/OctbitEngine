//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/File/Path.h>

namespace ob::platform {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class DynamicLibrary {

        class Function {
            friend class DynamicLibrary;
        public:

            Function() = default;

            operator bool()const {
                return m_ptr != nullptr;
            }

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
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        DynamicLibrary(const Path& path);
        ~DynamicLibrary();

        Function getFunction(StringView name);

    private:

        Path m_path;
        // WindowsではHMODULE
        const void* m_handle;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob
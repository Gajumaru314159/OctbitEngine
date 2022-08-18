//***********************************************************
//! @file
//! @brief		デリゲート基底クラス
//! @author		Gajumaru
//***********************************************************
//! @cond
#pragma once
#include <memory>

namespace ob::core {
    namespace detail {

        //@―---------------------------------------------------------------------------
        //! @brief ファンクタ
        //@―---------------------------------------------------------------------------
        struct Functor {
        private:

            class Base {
            public:virtual void Func()noexcept {}
            };
            class A :public virtual Base {};
            class B :public virtual Base {};
            class Derived :public A, public B { public:virtual void Func()noexcept override {} };

            enum {
                function_size = sizeof(&Derived::Func),
            };
            enum {
                buffer_size = 32,
            };
            static_assert(function_size < buffer_size, "buffer_size is too small.");

        public:

            Functor() = default;

            Functor& operator=(const Functor& src)noexcept;
            bool operator==(const Functor& rhs)const noexcept;
            bool operator!=(const Functor& rhs)const noexcept;

            void clear()noexcept;

            template<typename T>
            T* get_object()const;

            template<typename T>
            void set_object(T* pObject)noexcept;

            template<typename F>
            F get_function()const;

            template<typename F>
            void set_function(const F function)noexcept;

        private:

            void* m_pObject = nullptr;
            alignas(alignof(Derived)) char m_function[buffer_size] = {};

        };


        //@―---------------------------------------------------------------------------
        //! @brief デリゲート基底クラス
        //@―---------------------------------------------------------------------------
        class DelegateBase {
        protected:

            Functor m_functor;  //!< ファンクタ

        };






        //===============================================================
        // インライン関数
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief コピー演算子
        //@―---------------------------------------------------------------------------
        inline Functor& Functor::operator=(const Functor& src)noexcept {
            m_pObject = src.m_pObject;
            memcpy_s(m_function, sizeof(m_function), src.m_function, sizeof(m_function));
            return *this;
        }


        //@―---------------------------------------------------------------------------
        //! @brief 等価演算子
        //@―---------------------------------------------------------------------------
        inline bool Functor::operator==(const Functor& rhs)const noexcept {
            if (m_pObject != rhs.m_pObject)return false;
            return (std::memcmp((void*)m_function, (void*)rhs.m_function, sizeof(m_function)) == 0);
        }

        //@―---------------------------------------------------------------------------
        //! @brief 否等価演算子
        //@―---------------------------------------------------------------------------
        inline bool Functor::operator!=(const Functor& rhs)const noexcept {
            return !(*this == rhs);
        }


        //@―---------------------------------------------------------------------------
        //! @brief クリア
        //@―---------------------------------------------------------------------------
        inline void Functor::clear()noexcept {
            m_pObject = nullptr;
            memset(m_function, 0x00, sizeof(m_function));
        }


        //@―---------------------------------------------------------------------------
        //! @brief オブジェクトを取得
        //@―---------------------------------------------------------------------------
        template<typename T>
        inline T* Functor::get_object()const {
            return reinterpret_cast<T*>(m_pObject);
        }


        //@―---------------------------------------------------------------------------
        //! @brief オブジェクトを設定
        //@―---------------------------------------------------------------------------
        template<typename T>
        inline void Functor::set_object(T* pObject)noexcept {
            m_pObject = reinterpret_cast<void*>(pObject);
        }


        //@―---------------------------------------------------------------------------
        //! @brief 関数を取得
        //@―---------------------------------------------------------------------------
        template<typename F>
        inline F Functor::get_function()const {
            return *reinterpret_cast<const F*>(&m_function[0]);
        }


        //@―---------------------------------------------------------------------------
        //! @brief 関数を設定
        //@―---------------------------------------------------------------------------
        template<typename F>
        inline void Functor::set_function(const F function)noexcept {
            static_assert(sizeof(F) <= sizeof(m_function), "require more function size");
            memcpy_s(m_function, sizeof(m_function), &function, sizeof(F));
        }

    }


}// namespcae ob
//! @endcond
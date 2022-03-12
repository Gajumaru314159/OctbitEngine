//***********************************************************
//! @file
//! @brief		デリゲート
//! @author		Gajumaru
//***********************************************************
#pragma once
#include "Details/DelegateBase.h"

namespace ob {

    //! @cond
    template<class SIGNATURE>
    class delegate {};
    //! @endcond


    //@―---------------------------------------------------------------------------
    //! @brief      デリゲート
    //! 
    //! @details    インスタンスへの参照をメソッドへの参照をペアにしてカプセル化する。
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    class delegate<Ret(Args...)> :public detail::delegate_base
    {
    public:

        using this_type = typename delegate<Ret(Args...)>;                                      //!< this_type
        using return_type = Ret;                                                                //!< return_type
        using function_type = Ret(Args...);                                                     //!< function_type

        template<typename T>using method_type = Ret(T::*)(Args... args);                        //!< method_type
        template<typename T>using const_method_type = Ret(T::*)(Args... args)const;             //!< const_method_type

    private:

        using base_type = detail::delegate_base;
        using functor_type = detail::Functor;
        using invoke_type = Ret(const functor_type&, Args...);

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================
        constexpr delegate()noexcept;                                   // デフォルトコンストラクタ
        delegate(const this_type& rhs)noexcept;                         // コピーコンストラクタ
        delegate(this_type&& rhs)noexcept;                              // ムーブコンストラクタ
        delegate(function_type& function)noexcept;                      // 関数で初期化する
        template<class T>
        delegate(T& object, method_type<T> pMethod)noexcept;            // メンバ関数で初期化する
        template<class T>
        delegate(const T& object, const_method_type<T> pMethod)noexcept;// constメンバ関数で初期化する

        //@―---------------------------------------------------------------------------
        //! @brief ラムダ式による初期化
        //@―---------------------------------------------------------------------------
        //template<typename T, typename = enable_if_t <is_invocable_v<T,Args...>>>
        template<typename T, typename = enable_if_t <!is_base_of<this_type, remove_cvref_t<T>>::value && !is_base_of<function_type, remove_cvref_t<T>>::value>>
        delegate(T&& function)noexcept {
            assign(forward<T>(function));
        }


        //===============================================================
        // オペレータ
        //===============================================================
        constexpr bool operator==(const this_type& rhs)const noexcept;  // 等価演算子
        constexpr bool operator!=(const this_type& rhs)const noexcept;  // 否等価演算子
        this_type& operator=(const this_type& rhs)noexcept;             // コピー代入演算子
        Ret operator()(Args... args)const;                              // 登録された関数を呼び出す


        //===============================================================
        // 登録関数
        //===============================================================
        void assign(function_type& function)noexcept;                           // 関数を登録する
        template<class T>
        void assign(T& object, method_type<T> pMethod)noexcept;                 // メンバ関数を登録する
        template<class T>
        void assign(const T& object, const_method_type<T> pMethod)noexcept;     // constメンバ関数を登録する

        //@―---------------------------------------------------------------------------
        //! @brief ラムダ式を登録する
        //@―---------------------------------------------------------------------------
        template<typename T, typename = enable_if_t <!is_base_of<this_type, remove_cvref_t<T>>::value && !is_base_of<function_type, remove_cvref_t<T>>::value>>
        void assign(T&& function)noexcept {
            m_pInvoke = &invoke_lamda<remove_cvref_t<T>>;
            m_functor.set_object<void*>(nullptr);
            m_functor.set_function(forward<T>(function));
        }


        //===============================================================
        //
        //===============================================================
        constexpr void clear()noexcept;                                                 // 関数登録を解除する
        constexpr bool empty()const noexcept;                                           // 関数が登録されているか判定する


    private:
        
        void copy_impl(const this_type& rhs)noexcept;                                   // コピー
        static Ret invoke_nop(const functor_type&, Args...)noexcept;                    // 無効関数
        static Ret invoke_function(const functor_type& functor, Args... args);          // 登録された関数を呼び出す
        template<class T>
        static Ret invoke_method(const functor_type& functor, Args... args);            // メンバ関数を呼び出す
        template<class T>
        static Ret invoke_const_method(const functor_type& functor, Args... args);      // constメンバ関数を呼び出す
        template<class T>
        static Ret invoke_lamda(const functor_type& functor, Args... args);             // ラムダ式を呼び出す

    private:

        invoke_type* m_pInvoke = &this_type::invoke_nop;

    };

    using void_delegate_type = delegate<void(void)>;

    // noexcept版未対応


    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------
    //! @brief デフォルトコンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline constexpr delegate<Ret(Args...)>::delegate()noexcept {
        clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief コピーコンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline delegate<Ret(Args...)>::delegate(const this_type& rhs)noexcept {
        copy_impl(rhs);
    }


    //@―---------------------------------------------------------------------------
    //! @brief ムーブコンストラクタ
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline delegate<Ret(Args...)>::delegate(this_type&& rhs)noexcept {
        m_pInvoke = rhs.m_pInvoke;
        m_functor = ob::move(rhs.m_functor);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 関数で初期化する
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline delegate<Ret(Args...)>::delegate(function_type& function)noexcept {
        assign(function);
    }


    //@―---------------------------------------------------------------------------
    //! @brief メンバ関数で初期化する
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    template<class T>
    inline delegate<Ret(Args...)>::delegate(T& object, method_type<T> pMethod)noexcept {
        assign(object, pMethod);
    }


    //@―---------------------------------------------------------------------------
    //! @brief constメンバ関数で初期化する
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    template<class T>
    inline delegate<Ret(Args...)>::delegate(const T& object, const_method_type<T> pMethod)noexcept {
        assign(object, pMethod);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 等価演算子
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline constexpr bool delegate<Ret(Args...)>::operator==(const this_type& rhs)const noexcept {
        return (m_pInvoke == rhs.m_pInvoke) && (m_functor == rhs.m_functor);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 否等価演算子
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline constexpr bool delegate<Ret(Args...)>::operator!=(const this_type& rhs)const noexcept {
        return !(*this == rhs);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コピー代入演算子
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline delegate<Ret(Args...)>& delegate<Ret(Args...)>::operator=(const this_type& rhs)noexcept {
        if (this != &rhs)copy_impl(rhs);
        return *this;
    }


    //@―---------------------------------------------------------------------------
    //! @brief 登録された関数を呼び出す
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline Ret delegate<Ret(Args...)>::operator()(Args... args)const {
        return (*m_pInvoke)(m_functor, forward<Args>(args)...);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 関数を登録する
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline void delegate<Ret(Args...)>::assign(function_type& function)noexcept {
        m_pInvoke = &invoke_function;
        m_functor.set_object<void*>(nullptr);
        m_functor.set_function(&function);
    }


    //@―---------------------------------------------------------------------------
    //! @brief メンバ関数を登録する
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    template<class T>
    inline void delegate<Ret(Args...)>::assign(T& object, method_type<T> pMethod)noexcept {
        //assert(pMethod != nullptr);
        m_pInvoke = &invoke_method<T>;
        m_functor.set_object(&object);
        m_functor.set_function(pMethod);
    }


    //@―---------------------------------------------------------------------------
    //! @brief constメンバ関数を登録する
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    template<class T>
    inline void delegate<Ret(Args...)>::assign(const T& object, const_method_type<T> pMethod)noexcept {
        //assert(pMethod != nullptr);
        m_pInvoke = &invoke_const_method<T>;
        m_functor.set_object(const_cast<T*>(&object));
        m_functor.set_function(pMethod);
    }


    //@―---------------------------------------------------------------------------
    //! @brief 関数登録を解除する
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline constexpr void delegate<Ret(Args...)>::clear()noexcept {
        m_pInvoke = &invoke_nop;
        m_functor.clear();
    }


    //@―---------------------------------------------------------------------------
    //! @brief 関数が登録されているか判定する
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline constexpr bool delegate<Ret(Args...)>::empty()const noexcept {
        return (m_pInvoke == &invoke_nop);
    }


    //@―---------------------------------------------------------------------------
    //! @brief コピー
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline void delegate<Ret(Args...)>::copy_impl(const this_type& rhs)noexcept {
        m_pInvoke = rhs.m_pInvoke;
        m_functor = rhs.m_functor;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      無効関数
    //! 
    //! @details    関数が登録されていないときに呼び出されるダミー関数
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline Ret delegate<Ret(Args...)>::invoke_nop(const functor_type&, Args...)noexcept {
        // 何もしない
    }


    //@―---------------------------------------------------------------------------
    //! @brief      登録された関数を呼び出す
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    inline Ret delegate<Ret(Args...)>::invoke_function(const functor_type& functor, Args... args) {
        return (*(functor.get_function<function_type*>()))(forward<Args>(args)...);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      メンバ関数を呼び出す
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    template<class T>
    inline Ret delegate<Ret(Args...)>::invoke_method(const functor_type& functor, Args... args) {
        return ((functor.get_object<T>())->*(functor.get_function<method_type<T>>()))(forward<Args>(args)...);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      constメンバ関数を呼び出す
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    template<class T>
    inline Ret delegate<Ret(Args...)>::invoke_const_method(const functor_type& functor, Args... args) {
        return ((functor.get_object<T>())->*(functor.get_function<const_method_type<T>>()))(forward<Args>(args)...);
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ラムダ式を呼び出す
    //@―---------------------------------------------------------------------------
    template<typename Ret, typename... Args>
    template<class T>
    inline Ret delegate<Ret(Args...)>::invoke_lamda(const functor_type& functor, Args... args) {
        return (functor.get_function<T>())(forward<Args>(args)...);
    }


}// namespcae ob
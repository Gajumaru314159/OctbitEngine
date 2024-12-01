//***********************************************************
//! @file
//! @brief		ミニデリゲート
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <cassert>

namespace ob::core {

	//@ cond
	template<class>
	class MiniDalegate;
	//@ endcond

//! @brief      Ownerのみをキャプチャできる軽量デリゲート
//!	@details    通常のデリゲートよりも軽量ですが、ラムダ式のように変数をキャプチャすることはできません。
    template<class R, class... Args>
    class MiniDalegate<R(Args...)> {
    public:
        using StaticFuncPtr = R(*)(Args...);
        constexpr static size_t kBufferSize = 24; // T (8) + BaseInvoker (8) + Invoker (8) = 24

    public:
        //! @brief      デフォルトコンストラクタ
        //! @details    MiniDalegateクラスのデフォルトコンストラクタです。
        MiniDalegate() = default;

        //! @brief      コンストラクタ
        //! @param[in]  func  静的関数ポインタ
        MiniDalegate(StaticFuncPtr func) {
            assign(func);
        }

        //! @brief      コンストラクタ
        //! @param[in]  object  オブジェクトの参照
        //! @param[in]  func    メンバ関数ポインタ
        template<class T>
        MiniDalegate(T& object, R(T::* func)(Args...)) {
            assign(object, func);
        }

        //! @brief      コンストラクタ
        //! @param[in]  object  オブジェクトの参照
        //! @param[in]  func    メンバ関数ポインタ(const)
        template<class T>
        MiniDalegate(const T& object, R(T::* func)(Args...) const) {
            assign(object, func);
        }

        //! @brief      静的関数ポインタを割り当てる
        //! @param[in]  func  静的関数ポインタ
        void assign(StaticFuncPtr func) {
            static_assert(sizeof(StaticInvoker) <= kBufferSize, "buffer is too small");
            reset();
            new (m_buffer.data()) StaticInvoker(func);
        }

        //! @brief      オブジェクトとメンバ関数ポインタを割り当てる
        //! @param[in]  object  オブジェクトの参照
        //! @param[in]  func    メンバ関数ポインタ
        template<class T>
        void assign(T& object, R(T::* func)(Args...)) {
            static_assert(sizeof(Invoker<T>) <= kBufferSize, "buffer is too small");
            reset();
            new (m_buffer.data()) Invoker(object, func);
        }

        //! @brief      オブジェクトとメンバ関数ポインタを割り当てる(const)
        //! @param[in]  object  オブジェクトの参照
        //! @param[in]  func    メンバ関数ポインタ
        template<class T>
        void assign(const T& object, R(T::* func)(Args...) const) {
            static_assert(sizeof(ConstInvoker<T>) <= kBufferSize, "buffer is too small");
            reset();
            new (m_buffer.data()) ConstInvoker(object, func);
        }

        //! @brief      デリゲートが有効かどうかを判定する
        //! @return     デリゲートが有効な場合はtrue、そうでない場合はfalse
        operator bool() const {
            return std::equal_range(m_buffer.begin(), m_buffer.end(), 0);
        }

        //! @brief      デリゲートをリセットする
        void reset() {
            m_buffer.fill(0);
        }

        //! @brief      デリゲートを呼び出す
        //! @param[in]  args  関数引数
        //! @return     関数の戻り値
        R operator()(Args... args) const {
            return reinterpret_cast<const BaseInvoker*>(m_buffer.data())->invoke(args...);
        }

    private:

        struct BaseInvoker {
            virtual R invoke(Args... args) const = 0;
        };

        struct StaticInvoker : public BaseInvoker {
            StaticFuncPtr m_func;
            StaticInvoker(StaticFuncPtr func) : m_func(func) {}
            R invoke(Args... args) const {
                return m_func(args...);
            }
        };

        template<class T>
        struct Invoker : public BaseInvoker {
            using SIG = R(T::*)(Args...);
            T& m_owner;
            SIG m_method;
            Invoker(T& owner, SIG method) : m_owner(owner), m_method(method) {}
            R invoke(Args... args) const {
                return (m_owner.*m_method)(args...);
            }
        };

        template<class T>
        struct ConstInvoker : public BaseInvoker {
            using SIG = R(T::*)(Args...) const;
            const T& m_owner;
            SIG m_method;
            ConstInvoker(const T& owner, SIG method) : m_owner(owner), m_method(method) {}
            R invoke(Args... args) const {
                return (m_owner.*m_method)(args...);
            }
        };

    private:
        Array<size_t, kBufferSize / sizeof(size_t)> m_buffer;
    };

}
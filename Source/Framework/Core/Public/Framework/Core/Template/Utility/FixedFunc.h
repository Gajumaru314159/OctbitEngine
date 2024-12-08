//***********************************************************
//! @file
//! @brief		固定長関数オブジェクト
//! @author		Gajumaru
//! @reference	https://zenn.dev/suuta/articles/c4c47e8626d5aa
//! @reference	https://gist.github.com/Suuta/43b2e7ee973d8d3fda581e89efe28b45
//***********************************************************
#pragma once
#include <Framework/Core/Template/Utility/TypeTraits.h>

namespace ob::core {

	//! @cond 
	template <class Signature, size_t BufferSize>
	class FixedFunc;
	//! @endcond 

	//! @brief 固定長関数オブジェクト
	//! @details std::functionと異なりヒープアロケーションを行わない関数オブジェクト
	template <class R, class... Args, size_t BufferSize>
	class FixedFunc<R(Args...), BufferSize> {
	public:
		using result_type = R;
	public:

		//! @brief デフォルトコンストラクタ
		FixedFunc() = default;

		//! @brief デストラクタ
		~FixedFunc() { reset(); }

		//! @brief コピーコンストラクタ
		FixedFunc(const FixedFunc& other) {
			if (other.m_callable) {
				other.m_callable->clone(m_buffer, &m_callable);
			}
		}

		//! @brief ムーブコンストラクタ 
		FixedFunc(FixedFunc&& other) {
			if (other.m_callable) {
				other.m_callable->clone(m_buffer, &m_callable);
				other.reset();
			}
		}

		//! @brief コピー代入演算子
		FixedFunc& operator=(const FixedFunc& other) {
			if (&other == this)
				return *this;

			reset();

			if (other.m_callable) {
				other.m_callable->clone(m_buffer, &m_callable);
			}

			return *this;
		}

		//! @brief ムーブ代入演算子 
		FixedFunc& operator=(FixedFunc&& other) {
			if (&other == this)
				return *this;

			reset();

			if (other.m_callable) {
				other.m_callable->clone(m_buffer, &m_callable);
				other.reset();
			}

			return *this;
		}

		//! @brief 関数オブジェクトの登録
		template <class F,class = std::enable_if_t<
			// コピー/ムーブ除外
			(!std::is_base_of_v<FixedFunc,remove_cvr_t<F>>) &&
			// サイズチェック
			sizeof(Callable<F>) <= sizeof(m_buffer)
			>>
		FixedFunc operator=(F&& f) {
			if (m_callable)
				reset();

			m_callable = new (m_buffer) Callable<F>(std::forward<F>(f));

			return *this;
		}

		//! @brief 空の関数オブジェクトか
		operator bool() const { return m_callable; }

		//! @brief 関数オブジェクトの呼び出し
		R operator()(Args... args)const {
			return (*m_callable)(std::forward<Args>(args)...);
		}

		//! @brief 関数オブジェクトの解除
		void reset() {
			if (m_callable) {
				m_callable->~ICallable();
				m_callable = nullptr;
			}
			memset(m_buffer, 0, BufferSize + sizeof(void*));
		}

	private:

		struct ICallable {
			virtual ~ICallable() = default;
			virtual R operator()(Args...) const = 0;
			virtual void clone(u8* m_buffer, ICallable** callablePtr) const = 0;
		};

		template <class T>
		struct Callable : public ICallable {
			T functor;

			template<class F>
			Callable(F&& f) : functor(std::forward<F>(f)) {}

			R operator()(Args... args) const override {
				return functor(std::forward<Args>(args)...);
			}

			void clone(u8* m_buffer, ICallable** callablePtr) const override {
				*callablePtr = new (m_buffer) Callable(functor);
			}
		};

	private:
		ICallable*	m_callable = nullptr;
		u8			m_buffer[BufferSize + sizeof(void*)] = { 0 };
	};

}
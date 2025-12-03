//***********************************************************
//! @file
//! @author		Gajumaru
//! @reference	https://zenn.dev/suuta/articles/c4c47e8626d5aa
//! @reference	https://gist.github.com/Suuta/e294208c04f346103723a9df5e134d2f
//***********************************************************
#pragma once
#include <Framework/Core/Template/Utility/TypeTraits.h>
#include <Framework/Core/Template/Utility/FixedFunc.h>

namespace ob::core {

	//! @cond 
	template <class Signature, std::size_t BufferSize = 32>
	class Delegate;
	//! @endcond

	//! @brief デリゲート
	//! @details デリゲートは関数ポインタ、メンバ関数ポインタ、ラムダ式などを保持し、呼び出す。
	template <class R, class... Args, std::size_t BufferSize>
	class Delegate<R(Args...), BufferSize> {
	public:
		using function_type = FixedFunc<R(Args...), BufferSize>;
	public:

		//! @brief デフォルトコンストラクタ
		Delegate() = default;

		//! @brief インスタンスとメソッドの登録
		template <class T, class F>
		Delegate(T& obj, F mem_fn) {
			m_function = [&obj, mem_fn](Args... args) -> R { return (obj.*mem_fn)(std::forward<Args>(args)...); };
		}

		//! @brief 関数ポインタと関数オブジェクトの登録 
		template <class T, class = std::enable_if_t<!std::is_base_of_v<Delegate, remove_cvr_t<T>>>>
		Delegate(T&& f) {
			m_function =  std::forward<T>(f);
		}

		//! @brief 関数ポインタと関数オブジェクトの登録 
		template <class T, class = std::enable_if_t<!std::is_base_of_v<Delegate, remove_cvr_t<T>>>>
		Delegate& operator=(T&& f) {
			m_function = std::forward<T>(f);
			return *this;
		}

		//! @brief 空のデリゲートか
		operator bool() const { return m_function; }

		//! @brief デリゲートの呼出し 
		R operator()(Args... args) const {
			return m_function(std::forward<Args>(args)...);
		}

		//! @brief デリゲートをリセットする
		void reset() {
			m_function.reset();
		}

	private:
		function_type m_function;
	};

}
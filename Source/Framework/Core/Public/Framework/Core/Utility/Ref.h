//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

//! @cond

#define OB_SAFE_RETAIN(p)  \
    {                      \
        if (p) {           \
            (p)->retain(); \
        }                  \
    }

#define OB_SAFE_RELEASE(p)  \
    {                       \
        if (p) {            \
            (p)->release(); \
            (p) = nullptr;  \
        }                   \
    }

//! @endcond

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief		参照カウントオブジェクト
	//! @details	std::shared_ptr と異なり、参照カウントが0になったときの処理を
	//!				カスタマイズ可能です。
	//@―---------------------------------------------------------------------------
	class RefObject {
		template<class T> friend class Ref;
	protected:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//! @details	RefObjectを直接構築することは禁止されています。
		//@―---------------------------------------------------------------------------
		RefObject();

		//@―---------------------------------------------------------------------------
		//! @brief		デストラクタ
		//@―---------------------------------------------------------------------------
		virtual ~RefObject();

		//@―---------------------------------------------------------------------------
		//! @brief		参照カウントが0になったときの処理
		//! @details	デフォルトでは自オブジェクトの解放を行います。
		//!				オーバーロードする場合は解放漏れに注意してください。
		//@―---------------------------------------------------------------------------
		virtual void finalize();

	private:

		RefObject(const RefObject&) = delete;
		void operator=(const RefObject&) = delete;

		void retain();
		void release();

	private:
		Atomic<s32> m_referenceCount{ 0 };
	};

	//@―---------------------------------------------------------------------------
	//! @brief  RefObject 用スマートポインタ
	//@―---------------------------------------------------------------------------
	template<class T>
	class Ref {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief	空の Ref を構築 
		//@―---------------------------------------------------------------------------
		constexpr Ref() noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	空の Ref を構築 
		//@―---------------------------------------------------------------------------
		constexpr Ref(std::nullptr_t) noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	生ポインタから構築 
		//@―---------------------------------------------------------------------------
		Ref(T* ptr);

		//@―---------------------------------------------------------------------------
		//! @brief	コピーコンストラクタ
		//@―---------------------------------------------------------------------------
		Ref(const Ref& ref) noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	コピーコンストラクタ
		//@―---------------------------------------------------------------------------
		template<class Y>
		Ref(const Ref<Y>& ref) noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	ムーブコンストラクタ
		//@―---------------------------------------------------------------------------
		Ref(Ref&& ref) noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	ムーブコンストラクタ
		//@―---------------------------------------------------------------------------
		template<class Y>
		Ref(Ref<Y>&& ref) noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	デストラクタ
		//@―---------------------------------------------------------------------------
		~Ref();

		//@―---------------------------------------------------------------------------
		//! @brief	コピー代入
		//@―---------------------------------------------------------------------------
		Ref& operator=(const Ref& ref) noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	コピー代入
		//@―---------------------------------------------------------------------------
		template<class Y>
		Ref& operator=(const Ref<Y>& ref) noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	ムーブ代入
		//@―---------------------------------------------------------------------------
		Ref& operator=(Ref&& ref) noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	ムーブ代入
		//@―---------------------------------------------------------------------------
		template<class Y>
		Ref& operator=(Ref<Y>&& ref) noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	ポインタを間接参照する
		//@―---------------------------------------------------------------------------
		T& operator*() const noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	ポインタアクセス 
		//@―---------------------------------------------------------------------------
		T* operator->() const noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	有効なポインタを保持しているか 
		//@―---------------------------------------------------------------------------
		explicit operator bool() const noexcept;

		//@―---------------------------------------------------------------------------
		//! @brief	解放
		//@―---------------------------------------------------------------------------
		void reset();

		//@―---------------------------------------------------------------------------
		//! @brief	2つの Ref オブジェクトを入れ替える
		//@―---------------------------------------------------------------------------
		void swap(Ref<T>& other);

		//@―---------------------------------------------------------------------------
		//! @brief	保持しているオブジェクトへのポインタを取得 
		//@―---------------------------------------------------------------------------
		T* get() const;

		//@―---------------------------------------------------------------------------
		//! @brief	オブジェクトのポインタへの変換をサポート
		//! @note   ここでコンパイルエラーとなる場合、T の定義があるヘッダファイルを include しているか確認すること。
		//@―---------------------------------------------------------------------------
		operator T* () const;

		//@―---------------------------------------------------------------------------
		//! @brief	派生クラスへキャスト
		//@―---------------------------------------------------------------------------
		template<class Y>
		auto cast() const->std::enable_if_t<std::is_base_of<T, Y>::value, Y*>;

	private:
		RefObject* m_ptr = nullptr;
	};






	//===============================================================
	// インライン関数
	//===============================================================
	//! @cond

	//@―---------------------------------------------------------------------------
	//! @brief	空の Ref を構築 
	//@―---------------------------------------------------------------------------
	template<class T>
	constexpr Ref<T>::Ref() noexcept
	{
	}

	//@―---------------------------------------------------------------------------
	//! @brief	空の Ref を構築 
	//@―---------------------------------------------------------------------------
	template<class T>
	constexpr Ref<T>::Ref(std::nullptr_t) noexcept
	{
	}

	//@―---------------------------------------------------------------------------
	//! @brief	生ポインタから構築 
	//@―---------------------------------------------------------------------------
	template<class T>
	Ref<T>::Ref(T* ptr)
	{
		m_ptr = ptr;
		OB_SAFE_RETAIN(ptr);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	コピーコンストラクタ
	//@―---------------------------------------------------------------------------
	template<class T>
	Ref<T>::Ref(const Ref& ref) noexcept
	{
		m_ptr = ref.get();
		OB_SAFE_RETAIN(m_ptr);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	コピーコンストラクタ
	//@―---------------------------------------------------------------------------
	template<class T>
	template<class Y>
	Ref<T>::Ref(const Ref<Y>& ref) noexcept
	{
		T* t = ref.get();
		m_ptr = t;
		OB_SAFE_RETAIN(m_ptr);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	ムーブコンストラクタ
	//@―---------------------------------------------------------------------------
	template<class T>
	Ref<T>::Ref(Ref&& ref) noexcept
	{
		m_ptr = ref.m_ptr;
		ref.m_ptr = nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief	ムーブコンストラクタ
	//@―---------------------------------------------------------------------------
	template<class T>
	template<class Y>
	Ref<T>::Ref(Ref<Y>&& ref) noexcept
	{
		T* t = ref.get();
		m_ptr = t;
		ref.m_ptr = nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief	デストラクタ
	//@―---------------------------------------------------------------------------
	template<class T>
	Ref<T>::~Ref()
	{
		OB_SAFE_RELEASE(m_ptr);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	コピー代入
	//@―---------------------------------------------------------------------------
	template<class T>
	Ref<T>& Ref<T>::operator=(const Ref<T>& ref) noexcept
	{
		if (m_ptr != ref.m_ptr) {
			OB_SAFE_RETAIN(ref.m_ptr);
			OB_SAFE_RELEASE(m_ptr);
			m_ptr = ref.m_ptr;
		}
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief	コピー代入
	//@―---------------------------------------------------------------------------
	template<class T>
	template<class Y>
	Ref<T>& Ref<T>::operator=(const Ref<Y>& ref) noexcept
	{
		T* t = ref.get();   // 暗黙変換チェック
		if (m_ptr != ref.m_ptr) {
			OB_SAFE_RETAIN(m_ref.m_ptr);
			OB_SAFE_RELEASE(m_ptr);
			m_ptr = ref.m_ptr;
		}
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief	ムーブ代入
	//@―---------------------------------------------------------------------------
	template<class T>
	Ref<T>& Ref<T>::operator=(Ref&& ref) noexcept
	{
		if (&ref != this) {
			OB_SAFE_RELEASE(m_ptr);
			m_ptr = ref.m_ptr;
			ref.m_ptr = nullptr;
		}
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief	ムーブ代入
	//@―---------------------------------------------------------------------------
	template<class T>
	template<class Y>
	Ref<T>& Ref<T>::operator=(Ref<Y>&& ref) noexcept
	{
		OB_SAFE_RELEASE(m_ptr);
		T* t = ref.get();
		m_ptr = t;
		ref.m_ptr = nullptr;
		return *this;
	}

	//@―---------------------------------------------------------------------------
	//! @brief	ポインタを間接参照する
	//@―---------------------------------------------------------------------------
	template<class T>
	T& Ref<T>::operator*() const noexcept
	{
		OB_ASSERT_EXPR(m_ptr != nullptr);
		return *static_cast<T*>(m_ptr);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	ポインタアクセス 
	//@―---------------------------------------------------------------------------
	template<class T>
	T* Ref<T>::operator->() const noexcept
	{
		OB_ASSERT_EXPR(m_ptr != nullptr);
		return static_cast<T*>(m_ptr);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	有効なポインタを保持しているか 
	//@―---------------------------------------------------------------------------
	template<class T>
	Ref<T>::operator bool() const noexcept {
		return m_ptr != nullptr;
	}

	//@―---------------------------------------------------------------------------
	//! @brief	解放
	//@―---------------------------------------------------------------------------
	template<class T>
	void Ref<T>::reset()
	{
		OB_SAFE_RELEASE(m_ptr);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	2つの Ref オブジェクトを入れ替える
	//@―---------------------------------------------------------------------------
	template<class T>
	void Ref<T>::swap(Ref<T>& other)
	{
		if (&other != this) {
			T* t = m_ptr;
			m_ptr = other.m_ptr;
			other.m_ptr = t;
		}
	}

	//@―---------------------------------------------------------------------------
	//! @brief	保持しているオブジェクトへのポインタを取得 
	//@―---------------------------------------------------------------------------
	template<class T>
	T* Ref<T>::get() const
	{
		return static_cast<T*>(m_ptr);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	オブジェクトのポインタへの変換をサポート
	//! @note   ここでコンパイルエラーとなる場合、T の定義があるヘッダファイルを include しているか確認すること。
	//@―---------------------------------------------------------------------------
	template<class T>
	Ref<T>::operator T* () const {
		return static_cast<T*>(m_ptr);
	}

	//@―---------------------------------------------------------------------------
	//! @brief	派生クラスへキャスト
	//@―---------------------------------------------------------------------------
	template<class T>
	template<class Y>
	auto Ref<T>::cast() const->std::enable_if_t<std::is_base_of<T, Y>::value, Y*> {
		return static_cast<Y*>(m_ptr);
	}

	// 以下比較関数

	template<class T, class U>
	bool operator==(const Ref<T>& lhs, const Ref<U>& rhs) noexcept
	{
		return (lhs.get() == rhs.get());
	}

	template<class T>
	bool operator==(const Ref<T>& lhs, std::nullptr_t) noexcept
	{
		return (lhs.get() == nullptr);
	}

	template<class T>
	bool operator==(std::nullptr_t, const Ref<T>& rhs) noexcept
	{
		return (nullptr == rhs.get());
	}

	template<class T, class U>
	bool operator!=(const Ref<T>& lhs, const Ref<U>& rhs) noexcept
	{
		return (lhs.get() != rhs.get());
	}

	template<class T>
	bool operator!=(const Ref<T>& lhs, std::nullptr_t) noexcept
	{
		return (lhs.get() != nullptr);
	}

	template<class T>
	bool operator!=(std::nullptr_t, const Ref<T>& rhs) noexcept
	{
		return (nullptr != rhs.get());
	}

	template<class T, class U>
	bool operator<(const Ref<T>& lhs, const Ref<U>& rhs) noexcept
	{
		return (lhs.get() < rhs.get());
	}

	template<class T>
	bool operator<(const Ref<T>& lhs, std::nullptr_t) noexcept
	{
		return std::less<RefObject*>()(lhs.get(), nullptr);
	}

	template<class T>
	bool operator<(std::nullptr_t, const Ref<T>& rhs) noexcept
	{
		return std::less<T*>()(nullptr, rhs.get());
	}

	template<class T, class U>
	bool operator<=(const Ref<T>& lhs, const Ref<U>& rhs) noexcept
	{
		return (lhs.get() <= rhs.get());
	}

	template<class T>
	bool operator<=(const Ref<T>& lhs, std::nullptr_t) noexcept
	{
		return (lhs.get() <= nullptr);
	}

	template<class T>
	bool operator<=(std::nullptr_t, const Ref<T>& rhs) noexcept
	{
		return (nullptr <= rhs.get());
	}

	template<class T, class U>
	bool operator>(const Ref<T>& lhs, const Ref<U>& rhs) noexcept
	{
		return (lhs.get() > rhs.get());
	}

	template<class T>
	bool operator>(const Ref<T>& lhs, std::nullptr_t) noexcept
	{
		return (lhs.get() > nullptr);
	}

	template<class T>
	bool operator>(std::nullptr_t, const Ref<T>& rhs) noexcept
	{
		return (nullptr > rhs.get());
	}

	template<class T, class U>
	bool operator>=(const Ref<T>& lhs, const Ref<U>& rhs) noexcept
	{
		return (lhs.get() >= rhs.get());
	}

	template<class T>
	bool operator>=(const Ref<T>& lhs, std::nullptr_t) noexcept
	{
		return (lhs.get() >= nullptr);
	}

	template<class T>
	bool operator>=(std::nullptr_t, const Ref<T>& rhs) noexcept
	{
		return (nullptr >= rhs.get());
	}

	//! @endcond
}// namespcae ob

#undef OB_SAFE_RETAIN
#undef OB_SAFE_RELEASE
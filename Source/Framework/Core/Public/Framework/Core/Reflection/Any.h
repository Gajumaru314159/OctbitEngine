//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Core/Reflection/DynamicCast.h>
#include <Framework/Core/Template/Utility/Memory.h>
#include <Framework/Core/Template/Utility/TypeTraits.h>

namespace ob::core {

	class Any;
	struct TypeInfo;
	struct PropertyInfo;


	//! @brief プロパティ
	class Property {
	public:
		enum class Flag {
			Writable = get_bit(0)
		};
		using Flags = BitFlags<Flag>;
	public:

		//! @brief 空のプロパティを生成
		Property() = default;

		//! @brief 読み書き可能なプロパティを生成 
		Property(const TypeInfo& ownerInfo, void* owner, const PropertyInfo* info);

		//! @brief 読み取り専用プロパティを生成 
		Property(const TypeInfo& ownerInfo, const void* owner, const PropertyInfo* info);

		//! @brief コピー代入演算子
		Property& operator=(const Property& other) = default;

		//! @brief 値の代入演算子 
		Property& operator=(const Any& value) {
			assign(value);
			return *this;
		}

		//! @brief 値の代入演算子 
		template<class T>
		auto operator=(T&& value) -> std::enable_if_t<!std::is_same<remove_cvr_t<T>, Any>::value, Property&> {
			assign(Any(value));
			return *this;
		}

		Property& assign(const Any& value);

		//! @brief  プロパティが互換性のある型か
		bool is(const Type& to) const {
			return IsCastable(type(), to);
		}

		//! @brief  プロパティが互換性のある型か
		template<class T>
		bool is() const {
			return is(Type::Get<T>());
		}

		bool isReference() const;

		Any get();
		Any get() const;


		//! @brief  フォールバックを指定してプロパティを特定の特定の型の参照として取得する
		//! @details プロパティの型が異なる型であったり参照型でない場合はassertが発生します。
		template<class T>
		auto as() const -> std::enable_if_t<std::is_reference<T>::value,const T> {
			OB_ASSERT(!empty(), "空のプロパティです。");
			OB_ASSERT(is<T>(), "型が違います。is<T>()でアクセス可能な型か事前に確認してください。");
			OB_ASSERT(isReference(), "値型は参照型で受け取ることはできません。isReference()で参照可能か確認してください。");
			return get().as<std::remove_reference_t<T>>();
		}

		//! @brief  プロパティを特定の特定の型の参照として取得する
		template<class T>
		auto as(const T& fallback) const -> std::enable_if_t<std::is_reference<T>::value, const T> {
			if (empty() || !is<T>() || !isReference()) return fallback;
			return get().as<std::remove_reference_t<T>>();
		}

		//! @brief  プロパティを特定の特定の型のコピーとして取得する
		//! @details プロパティの型が異なる型の場合はassertが発生します。
		template<class T>
		auto as() const -> std::enable_if_t<!std::is_reference<T>::value && std::is_copy_assignable<T>::value, T> {
			OB_ASSERT(!empty(), "空のプロパティです。");
			OB_ASSERT(is<T>(), "型が違います。is<T>()でアクセス可能な型か事前に確認してください。");
			return get().as<T>();
		}

		//! @brief  フォールバックを指定してプロパティを特定の特定の型のコピーとして取得する
		template<class T>
		auto as(const T& fallback) const -> std::enable_if_t<!std::is_reference<T>::value && std::is_copy_assignable<T>::value, T> {
			if (empty() || !is<T>())return fallback;
			return get().as<T>();
		}

		Type type() const;

		Any copy()const;

		template<class T>
		T copy() const {
			OB_ASSERT(is<T>(), "型が違います");
			return get().as<T>();
		}

		//------ ラッパー ------//

		//! @brief プロパティのメンバを取得
		Any operator[](StringView name);
		Any operator[](StringView name) const;

		//! @brief プロパティのindex番目の要素を取得
		//Any operator[](size_t index) const {
		//	return copy()[index];
		//}
		
		bool empty() const {
			return m_owner == nullptr || m_info == nullptr;
		}

		void clear() {
			m_ownerInfo = nullptr;
			m_owner = nullptr;
			m_info = nullptr;
		}

		Any owner();
		Any owner()const;
	private:
		const TypeInfo* m_ownerInfo;
		void* m_owner;
		const PropertyInfo* m_info;
		Flags       m_flags;
	};

	class Any {
	public:
		enum class Flag {
			Reference = get_bit(0),
			Instance = get_bit(1),

			Writable = get_bit(2)
		};
		using Flags = BitFlags<Flag>;
	public:
		template<class T,class... Args>
		static Any Create(Args&&... args) {
			return { GetTypeInfo<T>(),new T(std::forward<Args>(args)...),Flag::Instance | Flag::Writable };
		}
	public:

		//! @brief 空のAnyオブジェクト作成
		Any();

		//! @brief デストラクタ
		~Any();

		//! @brief コピーコンストラクタ
		Any(const Any& other) {
			*this = other;
		}

		//! @brief ムーブコンストラクタ
		Any(Any&& other) noexcept{
			*this = other;
		}

		//! @brief コピー代入演算子
		Any& operator=(const Any& other);

		//! @brief ムーブ代入演算子
		Any& operator=(Any&& other) noexcept {
			m_info = other.m_info;
			m_pointer = other.m_pointer;
			m_reference = other.m_reference;
			m_writable = other.m_writable;
			other.clear();
			return *this;
		}

		//! @brief 参照Anyオブジェクトを生成 
		template<class T> Any(T& value) : Any(GetTypeInfo<T>(), &value, Flag::Reference | Flag::Writable) {}

		//! @brief 参照Anyオブジェクトを生成(const)
		template<class T> Any(const T& value) : Any(GetTypeInfo<T>(), &value, Flag::Reference) {}

		//! @brief コピーAnyオブジェクトを生成 
		template<class T> Any(T&& value) : Any(GetTypeInfo<T>(), new T(value), Flag::Instance) {}

		Any(const TypeInfo& info, const void* ptr, Flags flags) {
			m_info = &info;
			m_pointer = const_cast<void*>(ptr);
			m_reference = flags.has(Flag::Reference);
			m_writable = false;
			OB_ASSERT_EXPR(flags.has(Flag::Reference) || flags.has(Flag::Instance));
		}

		Any(const TypeInfo& info, void* ptr, Flags flags) {
			m_info = &info;
			m_pointer = ptr;
			m_reference = flags.has(Flag::Reference);
			m_writable = true;
			OB_ASSERT_EXPR(flags.has(Flag::Reference) || flags.has(Flag::Instance));
		}

		Property operator[](StringView name) const;

		Property operator[](StringView name);

		//Any operator[](size_t index) const {
		//	// return list()[index];
		//	// ListPropertyに対応するか？
		//	return {};
		//}
		//
		//Any operator[](size_t index) {
		//	return {};
		//}

		template<class T>
		auto operator=(T&& value) -> std::enable_if_t<!std::is_same<remove_cvr_t<T>, Any>::value,Any&> {
			if (m_pointer) {
				if (m_writable) {
					(*reinterpret_cast<remove_cvr_t<T>*>(m_pointer)) = value;
				}
			}
			return *this;
		}

		Any& assign(const Any& other);

		Type type() const;

		bool is(const Type& to) const {
			return IsCastable(type(), to);
		}

		template<class T>
		bool is() const {
			return is(Type::Get<T>());
		}

		template<class T>
		T& as() {
			OB_ASSERT(is<T>(), "型が違います");
			return *reinterpret_cast<T*>(m_pointer);
		}

		template<class T>
		const T& as() const {
			OB_ASSERT(is<T>(), "型が違います");
			return *reinterpret_cast<T*>(m_pointer);
		}

		template<class T>
		T& as(T& fallback) const {
			if (is<T>()) return *reinterpret_cast<T*>(m_pointer);
			return fallback;
		}

		template<class T>
		const T& as(const T& fallback) const {
			if (is<T>()) return *reinterpret_cast<T*>(m_pointer);
			return fallback;
		}

		bool isReference() const {
			return m_reference;
		}
		bool isWritable() const {
			return m_writable;
		}

		template<class T>
		Any& set(StringView name, T&& value) {
			operator[](name) = value;
			return *this;
		}

		template< class T, class... TNames>
		Any& set(StringView name, TNames&&... names, T value) {
			auto temp = operator[](name).copy();
			temp.set(names..., value);
			operator[](name) = temp;
			return *this;
		}

		bool empty() const {
			return m_pointer == nullptr || m_info == nullptr;
		}

		void clear() {
			m_info = nullptr;
			m_pointer = nullptr;
		}

		Any copy()const;

		template<class T>
		T copy()const {
			return T(as<T>());
		}

		template<class T>
		UPtr<T> release() {
			if (is<T>()) {
				auto pointer = m_pointer;
				clear();
				return UPtr<T>(static_cast<T*>(pointer));
			}
			return nullptr;
		}

		// Vector<Any> list();
		// Map<Any, Any> map();

		Flags flags()const;

		void seralize(BinaryWriter& writer) {}
		void deserialize(BinaryReader& reader) {}
	private:
		template<class T>
		static const TypeInfo& GetTypeInfo() { return GetTypeInfo(Type::Get<T>()); }
		static const TypeInfo& GetTypeInfo(const Type&);
	private:
		const TypeInfo* m_info = nullptr;
		void* m_pointer = nullptr;
		bool m_reference : 1;
		bool m_writable : 1;
	};

}
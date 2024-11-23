#include <Framework/Core/Reflection/Any.h>
#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Core/Reflection/TypeInfo.h>

namespace ob::core {

	//! @brief 読み書き可能なプロパティを生成 
	Property::Property(const TypeInfo& ownerInfo, void* owner, const PropertyInfo* info)
		: m_ownerInfo(&ownerInfo), m_owner(owner), m_info(info), m_writableOwner(Writable::Yes)
	{
		if (!info) {
			clear();
		}
	}

	//! @brief 読み取り専用プロパティを生成 
	Property::Property(const TypeInfo& ownerInfo, const void* owner, const PropertyInfo* info)
		: m_ownerInfo(&ownerInfo), m_owner(const_cast<void*>(owner)), m_info(info), m_writableOwner(Writable::No)
	{
		if (info) {
		}
		else {
			clear();
		}
	}

	//! @brief 値をAny型で設定
	Property& Property::assign(const Any& value) {
		if (m_info && m_ownerInfo) {
			if (isWritable()) {
				m_info->setter(Any(*m_ownerInfo,m_owner),value);
			}
		}
		return *this;
	}

	//! @brief 参照型のプロパティか
	bool Property::isReference() const {
		return m_info ? m_info->isReference : false;
	}

	//! @brief 書き込み可能なプロパティか
	//! @details Ownerがconst、もしくはプロパティのSetterがない場合はfalseを返します。
	bool Property::isWritable() const {
		if (!m_writableOwner)return false;
		return m_info ? m_info->canWrite() : false;
	}

	//! @brief PropertyをAny型で取得しなおす
	//! @details isReference()がfalseの場合はコピーを返します。
	Any Property::get() {
		return m_info ? m_info->getter(owner()):Any();
	}

	//! @brief PropertyをAny型で取得しなおす
	//! @details isReference()がfalseの場合はコピーを返します。
	//!		     取得したAnyオブジェクトは書き込み不可としてマークされます。
	Any Property::get() const {
		return m_info ? m_info->getter(owner()):Any();
	}

	//! @brief プロパティの型を取得
	Type Property::type() const {
		return m_info ? m_info->type : Type();
	}

	Any Property::copy()const {
		if (m_info) return m_info->getter(owner()).copy();
		return {};
	}

	Any Property::owner() {
		if (m_ownerInfo && m_owner) {
			if (m_writableOwner) {
				return Any(*m_ownerInfo, m_owner);
			} else {
				return Any(*m_ownerInfo, (const void*)m_owner);
			}
		}
		return {};
	}
	Any Property::owner()const {
		if (m_ownerInfo && m_owner) {
			return Any(*m_ownerInfo, (const void*)m_owner);
		}
		return {};
	}

	Any Property::operator[](StringView name) {
		if (empty())return {};
		if (isReference()) {
			return m_info->getter(owner())[name].get();
		}
		else {
			return copy()[name].copy();
		}
	}

	Any Property::operator[](StringView name) const {
		if (empty())return {};
		if (isReference()) {
			return m_info->getter(owner())[name].get();
		} else {
			return copy()[name].copy();
		}
	}




	Any::~Any() {
		if (m_info && !m_reference) m_info->destroy(m_pointer);
		clear();
	}

	//! @brief コピー代入演算子
	Any& Any::operator=(const Any& other) {
		if (other.empty()) return *this;
		m_info = other.m_info;
		m_reference = Reference::Yes;
		m_writable = other.m_writable;
		m_pointer = other.m_pointer;
		return *this;
	}

	//! @brief ムーブ代入演算子
	Any& Any::operator=(Any&& other) noexcept {
		m_info = other.m_info;
		m_pointer = other.m_pointer;
		m_reference = other.m_reference;
		m_writable = other.m_writable;
		other.clear();
		return *this;
	}

	Property Any::operator[](StringView name) const {
		if (m_info) {
			if (auto property = m_info->findProperty(name)) {
				return { *m_info, m_pointer ,property };
			}
		}
		return {};
	}

	Property Any::operator[](StringView name) {
		if (m_info) {
			if (auto property = m_info->findProperty(name)) {
				if (m_writable) {
					return { *m_info,  m_pointer ,property };
				}
				else {
					return { *m_info, (const void*)(m_pointer),property };
				}
			}
		}
		return {};
	}

	Any& Any::assign(const Any& other) {
		if (IsCastable(other.type(), type())) {
			m_info->assign(other.m_pointer, m_pointer);
		}
		return *this;
	}

	Type Any::type() const {
		return m_info ? m_info->type : Type();
	}


	Any Any::copy()const {
		if (empty())return {};
		auto instance = m_info->copy(m_pointer);
		if (instance == nullptr)return {};
		return Any(*m_info, instance, Reference::No,Writable::Yes);
	}

	const TypeInfo& Any::GetTypeInfo(const Type& type) {
		auto info = TypeInfo::Find(type);
		OB_ASSERT(info!=nullptr,"{}をリフレクション登録してください",type.name());
		return *info;
	}
}
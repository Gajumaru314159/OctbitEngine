#include <Framework/Core/Reflection/Any.h>
#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Core/Reflection/TypeInfo.h>

namespace ob::core {

	//! @brief 読み書き可能なプロパティを生成 
	Property::Property(const TypeInfo& ownerInfo, void* owner, const PropertyInfo* info)
		: m_ownerInfo(&ownerInfo), m_owner(owner), m_info(info), m_flags(Flag::Writable)
	{
		if (info) {
			if (info->canWrite() == false) {
				m_flags.off(Flag::Writable);
			}
		} else {
			clear();
		}
	}

	//! @brief 読み取り専用プロパティを生成 
	Property::Property(const TypeInfo& ownerInfo, const void* owner, const PropertyInfo* info)
		: m_ownerInfo(&ownerInfo), m_owner(const_cast<void*>(owner)), m_info(info), m_flags() 
	{
		if (info) {
		}
		else {
			clear();
		}
	}

	Property& Property::assign(const Any& value) {
		if (m_info && m_ownerInfo) {
			if (m_flags.has(Flag::Writable)) {
				m_info->setter(Any(*m_ownerInfo,m_owner, Any::Flag::Reference),value);
			}
		}
		return *this;
	}

	bool Property::isReference() const {
		return m_info ? m_info->isReference : false;
	}

	Any Property::get() {
		return m_info ? m_info->getter(owner()):Any();
	}
	Any Property::get() const {
		return m_info ? m_info->getter(owner()):Any();
	}

	Type Property::type() const {
		return m_info ? m_info->type : Type();
	}

	Any Property::copy()const {
		if (m_info) return m_info->getter(owner()).copy();
		return {};
	}

	Any Property::owner() {
		if (m_ownerInfo && m_owner) {
			Any::Flags flags = Any::Flag::Reference;
			if (m_flags.has(Flag::Writable)) flags |= Any::Flag::Writable;
			return Any(*m_ownerInfo, m_owner, flags);
		}
		return {};
	}
	Any Property::owner()const {
		if (m_ownerInfo && m_owner) {
			return Any(*m_ownerInfo, m_owner, Any::Flag::Reference);
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
		}
		else {
			return copy()[name].copy();
		}
	}



	Any::Any() {
		m_reference = false;
		m_writable = false;
	}

	Any::~Any() {
		if (m_info && !m_reference) m_info->destroy(m_pointer);
		clear();
	}

	//! @brief コピー代入演算子
	Any& Any::operator=(const Any& other) {
		if (other.empty()) return *this;
		m_info = other.m_info;
		m_reference = other.m_reference;
		m_writable = other.m_writable;
		m_pointer = m_reference ? other.m_pointer : m_info->copy(other.m_pointer);
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
					return { *m_info, static_cast<const void*>(m_pointer),property };
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
		return Any(*m_info, instance, Flag::Instance | Flag::Writable);
	}

	Any::Flags Any::flags()const {
		Flags flags;
		if (m_reference) flags |= Flag::Reference;
		if (!m_reference) flags |= Flag::Instance;
		if (m_writable) flags |= Flag::Writable;
		return flags;
	}

	const TypeInfo& Any::GetTypeInfo(const Type& type) {
		auto info = TypeInfo::Find(type);
		OB_ASSERT(info!=nullptr,"{}をリフレクション登録してください",type.name());
		return *info;
	}
}
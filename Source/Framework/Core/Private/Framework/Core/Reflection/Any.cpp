#include <Framework/Core/Reflection/Any.h>
#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Core/Reflection/TypeInfo.h>

namespace ob::core {

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
		return m_info->getter(owner());
	}
	Any Property::get() const {
		return m_info->getter(owner());
	}

	Type Property::type() const {
		return m_info ? m_info->type : Type();
	}

	Any Property::copy()const {
		if (m_info) return m_info->getter(owner());
		return {};
	}

	Any Property::owner() {
		if (m_ownerInfo && m_owner) {
			return Any(*m_ownerInfo, m_owner, Any::Flag::Reference);
		}
		return {};
	}
	Any Property::owner()const {
		if (m_ownerInfo && m_owner) {
			return Any(*m_ownerInfo, m_owner, Any::Flag::Reference);
		}
		return {};
	}

	Any::~Any() {
		if (m_info) m_info->destroy(m_pointer);
		clear();
	}

	//! @brief コピー代入演算子
	Any& Any::operator=(const Any& other) {
		if (other.empty()) return *this;
		m_info = other.m_info;
		m_flags = other.m_flags;
		m_pointer = other.m_flags.has(Flag::Reference) ? other.m_pointer : m_info->copy(other.m_pointer);
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
				if (m_flags.has(Flag::Writable)) {
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
			m_info->assign(m_pointer, other.m_pointer);
		}
		return *this;
	}

	Type Any::type() const {
		return m_info ? m_info->type : Type();
	}


	Any Any::copy()const {
		if (empty())return {};
		return Any(*m_info, m_info->copy(m_pointer), Flags(m_flags) |= Flag::Writable);
	}

	const TypeInfo& Any::GetTypeInfo(const Type& type) {
		auto info = TypeInfo::Find(type);
		OB_ASSERT(info!=nullptr,"{}をリフレクション登録してください",type.name());
		return *info;
	}
}
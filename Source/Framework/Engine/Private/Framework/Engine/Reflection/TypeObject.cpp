#include <Framework/Engine/Reflection/TypeObject.h>


namespace ob::engine::rtti {

	PropertyObject::PropertyObject(void* target, const PropertyInfo& info)
		: m_target(target)
		, m_info(info)
	{
		LOG_ERROR("{} {} のターゲットが未設定です。", m_info.type.name(), m_info.name);
	}

	void PropertyObject::setValueImpl(TypeId typeId, void* value) {
		if (m_target==nullptr) {
			LOG_ERROR("{} {} のターゲットが未設定です。", m_info.type.name(), m_info.name);
			return;
		}
		if (typeId != m_info.type) {
			LOG_ERROR("{} {} に{}型の値を設定しようとしました。",m_info.type.name(),m_info.name,typeId.name());
			return;
		}
		if (!m_info.getter) {
			LOG_ERROR("{} {} は読み取り専用です。", m_info.type.name(), m_info.name);
			return;
		}
		m_info.setter(m_target, value);
	}

	const void* PropertyObject::getValueImpl(TypeId typeId)const {
		if (m_target == nullptr) {
			LOG_ERROR("{} {} のターゲットが未設定です。", m_info.type.name(), m_info.name);
			return nullptr;
		}
		if (!m_info.getter) {
			LOG_ERROR("{} {} の取得関数が設定されていません。", m_info.type.name(), m_info.name);
			return nullptr;
		}
		return m_info.getter(m_target);
	}

}
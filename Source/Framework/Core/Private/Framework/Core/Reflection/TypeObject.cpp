#include <Framework/Core/Reflection/TypeObject.h>


namespace ob::core::rtti {

	PropertyObject::PropertyObject(void* target, const PropertyInfo& info)
		: m_target(target)
		, m_info(info)
	{
		LOG_ERROR("{} {} のターゲットが未設定です。", m_info.type.fullName(), m_info.name);
	}

	void PropertyObject::setValueImpl(Type type, TypedValue value) {
		if (m_target==nullptr) {
			LOG_ERROR("{} {} のターゲットが未設定です。", m_info.type.fullName(), m_info.name);
			return;
		}
		if (type != m_info.type) {
			LOG_ERROR("{} {} に{}型の値を設定しようとしました。",m_info.type.fullName(),m_info.name,type.fullName());
			return;
		}
		if (!m_info.getter) {
			LOG_ERROR("{} {} は読み取り専用です。", m_info.type.fullName(), m_info.name);
			return;
		}
		m_info.setter(m_target, value);
	}

	const TypedValue PropertyObject::getValueImpl(Type type)const {
		if (m_target == nullptr) {
			LOG_ERROR("{} {} のターゲットが未設定です。", m_info.type.fullName(), m_info.name);
			return {};
		}
		if (!m_info.getter) {
			LOG_ERROR("{} {} の取得関数が設定されていません。", m_info.type.fullName(), m_info.name);
			return {};
		}
		return m_info.getter(m_target);
	}

}
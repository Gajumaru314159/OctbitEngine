//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/PropertyNotifier.h>

namespace ob::engine {

	class ComponentCreator {
	public:
		virtual ~ComponentCreator() = default;
		virtual Component* createComponent() = 0;
		virtual Type getComponentType()const = 0;
		virtual Vector<Type> getDependentComponentTypes()const = 0;
	};

	template<class T>
	class ComponentCreatorTemplate : public ComponentCreator {
	public:
		ComponentCreatorTemplate() = default;
		Component* createComponent() override {
			return new T;
		}
		Type getComponentType()const override {
			return Type::Get<T>();
		}
		Vector<Type> getDependentComponentTypes()const {
			return {};
		}
	};


}

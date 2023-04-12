//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeId.h>
#include <Framework/Engine/Forward.h>
#include <Framework/Engine/INotifyPropertyChanged.h>

namespace ob::engine {

	class ComponentCreator {
	public:
		virtual ~ComponentCreator() = default;
		virtual Component* createComponent() = 0;
		virtual TypeId getComponentTypeId()const = 0;
		virtual Array<TypeId> getDependentComponentTypes()const = 0;
	};

	template<class T>
	class ComponentCreatorTemplate : public ComponentCreator {
	public:
		//Component* createComponent() override {
		//	return new T;
		//}
		TypeId getComponentTypeId()const override {
			return TypeId::Get<T>();
		}
		Array<TypeId> getDependentComponentTypes()const {
			return {};
		}
	};


}

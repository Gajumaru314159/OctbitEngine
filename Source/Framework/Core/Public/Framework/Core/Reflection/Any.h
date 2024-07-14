//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Type.h>
#include <Framework/Core/Reflection/AnyReference.h>
#include <Framework/Core/Template/Utility/Memory.h>

namespace ob::core {

    class Any {
    public:
        Any() {}

        template<typename ValueType>
        Any(const ValueType& value) : m_holder(new Holder<ValueType>(value)) {}

        template<typename ValueType>
        Any(UPtr<ValueType> value) : m_holder(new Holder<ValueType>(std::move(value))) {}

        Any(const Any& other) : m_holder(other.m_holder ? other.m_holder->clone() : nullptr) {}

        Any& operator=(const Any& other) {
            if (this != &other) {
                m_holder = other.m_holder ? other.m_holder->clone() : nullptr;
            }
            return *this;
        }

        template<typename ValueType>
        Any& operator=(const ValueType& value) {
            m_holder = new Holder<ValueType>(value);
            return *this;
        }

        operator ConstAnyReference () const {
            return m_holder ? m_holder->refelence() : ConstAnyReference();
        }

        bool empty() const {
            return !m_holder;
        }

        Type type() const {
            return m_holder ? m_holder->type() : Type();
        }

        void reset() {
            m_holder.reset();
        }

        template<typename ValueType>
        const ValueType& get() const {
            if (Type::Get<ValueType>() != type()) throw std::bad_cast();
            return *static_cast<Any::Holder<ValueType>*>(m_holder.get())->value;
        }

        template<class ValueType>
        UPtr<ValueType> release() {
            if (Type::Get<ValueType>() != type()) throw std::bad_cast();
            UPtr<ValueType> result = std::move(static_cast<Any::Holder<ValueType>*>(m_holder.get())->value);
            reset();
            return std::move(result);
        }

    private:
        class HolderBase {
        public:
            virtual ~HolderBase() {}
            virtual Type type() const = 0;
            virtual UPtr<HolderBase> clone() const = 0;
            virtual ConstAnyReference refelence() const = 0;
        };

        template<typename ValueType>
        class Holder : public HolderBase {
        public:
            Holder(const ValueType& value) : value(std::make_unique<ValueType>(value)) {}
            Holder(UPtr<ValueType> value) : value(std::move(value)) {}
            Type type() const override{
                return Type::Get<ValueType>();
            }
            UPtr<HolderBase> clone() const override{
                return UPtr<HolderBase>(new Holder(*value));
            }
            ConstAnyReference refelence() const override {
                return ConstAnyReference(*value);
            }
            UPtr<ValueType> value;
        };

    private:
        UPtr<HolderBase> m_holder;
    };



}
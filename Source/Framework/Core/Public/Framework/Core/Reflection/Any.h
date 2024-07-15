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

    //@―---------------------------------------------------------------------------
    //! @brief		コピー可能なあらゆる値を保持できる記憶型
    //@―---------------------------------------------------------------------------
    class Any {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief		デフォルトコンストラクタ
        //@―---------------------------------------------------------------------------
        Any() {}

        //@―---------------------------------------------------------------------------
        //! @brief		値からコピーを生成
        //@―---------------------------------------------------------------------------
        template<typename ValueType>
        Any(const ValueType& value) : m_holder(new Holder<ValueType>(value)) {}

        //@―---------------------------------------------------------------------------
        //! @brief		UPtrから直接構築
        //@―---------------------------------------------------------------------------
        template<typename ValueType>
        Any(UPtr<ValueType> value) : m_holder(new Holder<ValueType>(std::move(value),0)) {}

        //@―---------------------------------------------------------------------------
        //! @brief		コピー
        //@―---------------------------------------------------------------------------
        Any(const Any& other) : m_holder(other.m_holder ? other.m_holder->clone() : nullptr) {}

        //@―---------------------------------------------------------------------------
        //! @brief		コピー代入
        //@―---------------------------------------------------------------------------
        Any& operator=(const Any& other) {
            if (this != &other) m_holder = other.m_holder ? other.m_holder->clone() : nullptr;
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief		値からコピー代入
        //@―---------------------------------------------------------------------------
        template<typename ValueType>
        Any& operator=(const ValueType& value) {
            m_holder.reset(new Holder<ValueType>(value));
            return *this;
        }

        //@―---------------------------------------------------------------------------
        //! @brief		ConstAnyReferenceにキャスト
        //@―---------------------------------------------------------------------------
        operator bool () const {
            return !empty();
        }

        //@―---------------------------------------------------------------------------
        //! @brief		ConstAnyReferenceにキャスト
        //@―---------------------------------------------------------------------------
        operator AnyReference () const {
            return m_holder ? m_holder->refelence() : AnyReference();
        }

        //@―---------------------------------------------------------------------------
        //! @brief		ConstAnyReferenceにキャスト
        //@―---------------------------------------------------------------------------
        operator ConstAnyReference () const {
            return m_holder ? m_holder->refelence() : ConstAnyReference();
        }

        //@―---------------------------------------------------------------------------
        //! @brief		要素が空か
        //@―---------------------------------------------------------------------------
        bool empty() const {
            return !m_holder;
        }

        //@―---------------------------------------------------------------------------
        //! @brief		内部オブジェクトを解放する
        //@―---------------------------------------------------------------------------
        void reset() {
            m_holder.reset();
        }

        //@―---------------------------------------------------------------------------
        //! @brief		内部に保持している値の型を取得
        //@―---------------------------------------------------------------------------
        Type type() const {
            return m_holder ? m_holder->type() : Type();
        }

        //@―---------------------------------------------------------------------------
        //! @brief		内部型を取得する
        //@―---------------------------------------------------------------------------
        bool is(const Type&) const;
        template<typename ValueType>
        bool is() const { return is(Type::Get<ValueType>()); }

        //@―---------------------------------------------------------------------------
        //! @brief		内部オブジェトを取得する
        //@―---------------------------------------------------------------------------
        template<typename ValueType>
        ValueType& get() const {
            if (!is<ValueType>()) throw std::bad_cast();
            return *static_cast<Any::Holder<ValueType>*>(const_cast<HolderBase*>(m_holder.get()))->value;
        }

        //@―---------------------------------------------------------------------------
        //! @brief		内部オブジェクトをAny型の管理から解放してUPtrとして取得する
        //@―---------------------------------------------------------------------------
        template<class ValueType>
        UPtr<ValueType> release() {
            if (!is<ValueType>()) throw std::bad_cast();
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
            virtual AnyReference refelence() const = 0;
        };

        template<typename ValueType>
        class Holder : public HolderBase {
        public:
            Holder(const ValueType& value) : value(new ValueType(value)) {
                anyRef = *this->value;
            }
            Holder(UPtr<ValueType> value,int) : value(std::move(value)) {
                anyRef = *this->value;
            }
            Type type() const override{
                return Type::Get<ValueType>();
            }
            UPtr<HolderBase> clone() const override{
                if constexpr (std::is_copy_constructible<ValueType>::value) {
                    return UPtr<HolderBase>(new Holder(*value));
                } else {
                    throw std::exception("コピー不可なクラスです。");
                }
            }
            AnyReference refelence() const override {
                return anyRef;
            }
            AnyReference anyRef;
            UPtr<ValueType> value;
        };

    private:
        UPtr<HolderBase> m_holder;
    };



}
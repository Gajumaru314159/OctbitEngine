//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//! @note       UnrealEngineのUSubsystem/FSubsytemCollection参考
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {


    class SubsystemCollection {
    public:
        void* get(size_t type)const {
            return m_subsystemMap.at(type);
        }
    public:

    private:
        Map<size_t, class Subsystem*> m_subsystemMap;
        static Set<size_t> m_registeredSubsystems;

    };

    class Subsytem {
    private:
        friend class SubsystemCollection;
        SubsystemCollection* m_pOwner;
    };


    class TestOwner{
    public:

        template<typename T>
        T* getSubsystem() {
            return reinterpret_cast<T*>(getSubsystemImpl(typeof(t)));
        }
    private:
        void* getSubsystemImpl(size_t type)const {
            return m_subsystemCollection.get(type);
        }
    private:
        SubsystemCollection m_subsystemCollection;
    };



    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob
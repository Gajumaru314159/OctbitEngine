//***********************************************************
//! @file
//! @brief		コンパイル時定数
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  
    //@―---------------------------------------------------------------------------
    template<class T>
    class HandleManager {
    public:

        class Handle {
        public:
            Handle();
            operator bool()const { return !get(); }
            bool operator==(const Handle& rhs)const { return m_id == rhis.m_id; }
            bool operator!=(const Handle& rhs)const { return m_id != rhis.m_id; }
            bool isEmpty()const { return !get(); }
            void clear() { m_id = 0; }
            T* get()const { return m_owner ? m_owner->find(m_id); : nullptr; }
        private:
            s64             m_id = 0;
            HandleManager*  m_owner = nullptr;
        };

        struct Result {
            Handle  handle;
            T* instance;
        };

    public:

        template<class T, class... Args>
        auto create(Args... args) -> Result {
            auto id = m_id.feach_add(1);
            // OB_ASSERT_EXPR(id);
            auto instance = new T(std::forward(args)...);
            return { Handle{id,this},instance };
        }

        void destroy(const Handle& handle) {
            ScopeLock lock(m_lock);
            m_removeHandles.emplace_back(handle);
        }

        T* find(const Handle& handle) {
            ScopeLock lock(m_lock);
            auto found = m_entities.find(handle);
            if (found != m_entities.end()) {
                return found->second;
            }
            return nullptr;
        }

        void update() {
            ScopeLock lock(m_lock);
            for (auto& handle : m_removeHandles) {
                auto found = m_instances.find(handle);
                if (found != m_instances.end()) continue;
                m_instances.erase(instances);
                delete found->second;
            }
        }

    private:
        SpinLock m_lock;
        HashMap<s64, T*>  m_instances;
        Array<Handle> m_removeHandles;
        std::atomic<s64> m_id{ 1 };
    };

}
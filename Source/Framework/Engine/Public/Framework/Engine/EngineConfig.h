//***********************************************************
//! @file
//! @brief		エンジンコンフィグ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/TypeId.h>

namespace ob::engine {

    //@―---------------------------------------------------------------------------
    //! @brief      エンジンコンフィグ
    //! 
    //! @dettails   エンジンの起動設定やモジュールの設定を追加する。
    //@―---------------------------------------------------------------------------
    class EngineConfig {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  設定追加
        //@―---------------------------------------------------------------------------
        template<typename T>
        void set(T&& setting);

        //@―---------------------------------------------------------------------------
        //! @brief  設定取得
        //@―---------------------------------------------------------------------------
        template<typename T>
        const T& get() const;

    public:
        //! @cond 
        EngineConfig() = default;
        EngineConfig(const EngineConfig& rhs) noexcept { m_configs = rhs.m_configs; }
        EngineConfig(EngineConfig&& rhs) noexcept { m_configs = std::move(rhs.m_configs);}
        EngineConfig& operator = (const EngineConfig& rhs) noexcept { m_configs = rhs.m_configs; return *this; }
        EngineConfig& operator = (EngineConfig&& rhs) noexcept { m_configs = std::move(rhs.m_configs); return *this; }
        //! @endcond 
    private:

        mutable SpinLock m_lock;

        // TODO TypeIdを使用したAnyを実装する
        mutable HashMap<TypeId, std::any> m_configs;

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond

    //@―---------------------------------------------------------------------------
    //! @brief  設定追加
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline void EngineConfig::set(T&& setting) {
        ScopeLock lock(m_lock);
        m_configs[TypeId::Get<T>()] = std::forward<T>(setting);
    }

    //@―---------------------------------------------------------------------------
    //! @brief  設定取得
    //@―---------------------------------------------------------------------------
    template<typename T>
    inline const T& EngineConfig::get() const {
        ScopeLock lock(m_lock);
        auto& item = m_configs[TypeId::Get<T>()];
        if (item.has_value() == false) {
            item = T{};
        }
        return std::any_cast<const T&>(item);
    }

    //! @endcond
}// namespcae ob
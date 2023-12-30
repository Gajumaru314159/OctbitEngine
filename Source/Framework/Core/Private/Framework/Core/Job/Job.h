//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Template/include.h>
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Core/Job/JobHandle.h>

namespace ob::core {

    class JobGroup;

    //@―---------------------------------------------------------------------------
    //! @brief Job
    //@―---------------------------------------------------------------------------
    class Job {
    public:
        Job(JobGroup&,StringView name,Action&& action);
        ~Job();

        auto getName()const->const String&;

        void execute();

        void setSuspend(bool);
        bool isSuspended()const;

        void requestRelease();

    private:
        JobGroup&       m_group;
        String          m_name;
        Atomic<bool>    m_isSuspended;
        Func<void()>    m_action;
    };

}
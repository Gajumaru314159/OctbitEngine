//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Job/Job.h>
#include <Framework/Core/Job/JobGroup.h>

namespace ob {

    Job::Job(JobGroup& group, StringView name,Action&& action)
        : m_group(group)
        , m_name(name)
        , m_action(action)
        , m_isSuspended(true)
    {

    }

    //@―---------------------------------------------------------------------------
    //! @brief      デストラクタ
    //! @details    デストラクタは必ずフレームの最初に行われる
    //@―---------------------------------------------------------------------------
    Job::~Job() {
        // 削除順は JobHandle::~JobHandle → Job::requestRelease / Job::~Job
        m_group.removeJob(*this);
    }

    auto Job::getName()const->const String& {
        return m_name;
    }

    void Job::execute() {
        if (m_action) {
            m_action();
        }
    }
    void Job::setSuspend(bool value) {
        m_isSuspended = value;
    }
    bool Job::isSuspended()const {
        return m_isSuspended;
    }

    void Job::requestRelease() {
        setSuspend(true);
        m_group.removeJob(*this);
    }

}
//***********************************************************
//! @file
//! @brief		ハードウェア共通デファイン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Template/include.h>
#include <Framework/Core/Utility/Ref.h>
#include <Framework/Core/Job/JobHandle.h>

namespace ob::core {

    class JobExecutor;
    class Job;

    template<class T>
    class SyncContainer {
    public:

        void update() {
            ScopeLock lock(m_lock);
            // 追加
            for (auto& item : m_entryItems) {
                m_items.push_back(std::move(item));
            }
            // 削除
            for (auto itr = m_items.begin(); itr != m_items.end(); ) {
                if (m_leaveItems.count(itr->get())) {
                    itr = m_items.erase(itr);
                } else {
                    itr++;
                }
            }
        }

        void entry(UPtr<T>&& item) {
            ScopeLock lock(m_lock);
            m_entryItems.push_back(item);
        }

        void leave(const T* item) {
            ScopeLock lock(m_lock);
            m_leaveItems.emplace(item);
        }

        SpinLock            m_lock;
        Array<const T*>     m_items;
        Array<const T*>     m_entryItems;
        HashSet<const T*>   m_leaveItems;
    };

    //@―---------------------------------------------------------------------------
    //! @brief		ジョブ・グループ
    //! @details    JobGroupは実行依存関係を持ちます。
    //!             同期ポイントはEntryとLeaveがあります。
    //@―---------------------------------------------------------------------------
    class JobGroup {
    public:

        ~JobGroup();

        auto getName()const->const String&;

        auto createSub(StringView name) -> Ref<JobGroup>;
        auto createChild(StringView name) -> Ref<JobGroup>;
        void addChild(Ref<JobGroup>& group);

        auto addJob(StringView name, Action&& action)-> JobHandle&;


    public:

        void execute(JobExecutor& executor);
        void update(JobExecutor& executor);

        void removeJob(Job&);

        void requestRelease();

    private:
        JobGroup(JobGroup& parent,StringView name);
        void onLeaveSubGroup(JobExecutor& executor);

        s32 countLeafGroup()const;

    private:

        String                  m_name;

        SyncContainer<Job>      m_syncJobs;
        SyncContainer<JobGroup> m_syncGroups;

        // 所有
        Array<Job*>        m_jobs;
        Array<JobGroup*>   m_subGroups;
        Array<JobGroup*>   m_childGroups;

        // 追加予約
        SpinLock                m_entryLock;
        Array<UPtr<JobGroup>>   m_entrySubGroups;
        Array<UPtr<JobGroup>>   m_entryChildGroups;
        Array<UPtr<Job>>        m_entryJobs;

        // 解放予約
        HashSet<Job*>           m_leaveJobs;
        HashSet<JobGroup*>      m_leaveGroups;


        // 実依存
        Array<JobGroup*>        m_deps;



        Atomic<s32>             m_jobIndex;
        Atomic<s32>             m_childIndex;
        Atomic<s32>             m_executedNum;

        Atomic<s32>             m_dependParentCount;
        Atomic<s32>             m_dependSubCount;
    };

}
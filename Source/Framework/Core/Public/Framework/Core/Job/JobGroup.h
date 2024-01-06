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
    class JobSystem;

    //@―---------------------------------------------------------------------------
    //! @brief		ジョブ・グループ
    //! @details    JobGroupは実行依存関係を持ちます。
    //!             同期ポイントはEntryとLeaveがあります。
    //@―---------------------------------------------------------------------------
    class JobGroup {
        friend class JobSystem;
    public:

        //@―---------------------------------------------------------------------------
        //! @brief コンストラクタ
        //@―---------------------------------------------------------------------------
        JobGroup(JobSystem& sys, StringView name);

        //@―---------------------------------------------------------------------------
        //! @brief デストラクタ
        //@―---------------------------------------------------------------------------
        ~JobGroup();

        //@―---------------------------------------------------------------------------
        //! @brief 名前を取得
        //@―---------------------------------------------------------------------------
        auto getName()const->const String&;

        //@―---------------------------------------------------------------------------
        //! @brief サブ JobGroup を作成
        //@―---------------------------------------------------------------------------
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
        JobGroup(JobSystem& system,StringView name);
        void onLeaveSubGroup(JobExecutor& executor);

        s32 countLeafGroup()const;

    private:

        String                  m_name;
        JobSystem& m_system;

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
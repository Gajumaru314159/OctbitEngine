//***********************************************************
//! @file
//! @brief		TLSFアロケータ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Allocator/Heap.h>
#include <Framework/Core/Thread/Mutex.h>

namespace ob::core {

    //@―---------------------------------------------------------------------------
    //! @brief  Two-Level Segregate Fitアロケータ
    //! 
    //! @details http://www.gii.upv.es/tlsf/
    //!          https://www.researchgate.net/publication/234785757_A_comparison_of_memory_allocators_for_real-time_applications
    //@―---------------------------------------------------------------------------
    class TLSFHeap final :public Heap {
        friend class Heap;
    public:

        struct BlockTag;    // TLSFブロック・タグ

    public:

        //===============================================================
        //  コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief                  デストラクタ
        //@―---------------------------------------------------------------------------
        ~TLSFHeap() override;


        //@―---------------------------------------------------------------------------
        //! @brief                  内部アロケート情報のクリア
        //@―---------------------------------------------------------------------------
        void release() override;


        //===============================================================
        //  メモリ確保 /解放
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief	                メモリの確保
        //! 
        //! @param[in] size         確保サイズ
        //! @param[in] alignment    アライメント
        //! @param[in] zeroClear    確保したバッファをゼロで初期化するか
        //@―---------------------------------------------------------------------------
        void* allocate(size_t size, u32 alignment, bool zeroClear) override;


        //@―---------------------------------------------------------------------------
        //! @brief	                メモリの開放
        //@―---------------------------------------------------------------------------
        void deallocate(void* pBuffer) override;


        //===============================================================
        //  取得
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief	                ヒープ名を取得
        //@―---------------------------------------------------------------------------
        Char* name() const override;


        //@―---------------------------------------------------------------------------
        //! @brief	                アロケータのヒープサイズを取得
        //@―---------------------------------------------------------------------------
        size_t heapSize() const override;


        //@―---------------------------------------------------------------------------
        //! @brief	                アロケータの使用可能なヒープサイズを取得
        //@―---------------------------------------------------------------------------
        size_t freeHeapSize() const override;


        //@―---------------------------------------------------------------------------
        //! @brief	                利用可能な状態かどうか
        //@―---------------------------------------------------------------------------
        bool isValid()const override;

    protected:

        //@―---------------------------------------------------------------------------
        //! @brief	                コンストラクタ
        //@―---------------------------------------------------------------------------
        TLSFHeap(size_t size);

    private:

        void Clear();

        void Init(size_t size, s32 maxExpectDivideNum = -1);
        s32 GetMSB(u32 value);
        s32 GetLSB(u32 value);
        byte GetSLI(size_t size, byte fli);
        byte GetFreeListSLI(byte fli, byte sli);
        byte GetFreeListFLI(byte fli);
        byte GetFIndex(size_t size);
        byte GetFIndex(byte fli, byte sli);
        s32 GetBitCount(u32 value);

    private:

        using lock_type = ScopeLock<Mutex>;
        Mutex m_mutex;

        Char* m_pName;

        size_t m_allSize;          //
        s32 m_allAlocSize;
        s32 m_maxDivideNum;
        byte* m_pBaseMem;
        BlockTag* m_freeList;
        BlockTag* m_activeList;

        u32 m_freeListBitFLI;
        byte* freeListBitSLI;
        bool* freeListBit;
        bool* activeListBit;
        s32 BNS;
        s32 BBS;

        static const size_t s_blockManageSize;
        static const size_t s_blockMinimumSize;

        static const s32    s_secendLevelDivCount;
        static const s32    s_secendLevelDivNum;

        static const s32    s_linerManagementIndex;
        static const s32    s_linerManagementSize;
        static const s32    s_linerManagementDivSize;
    };


    //! @cond
    //@―---------------------------------------------------------------------------
    //! @brief  TLSFブロック・タグ
    //! 
    //! @ref    TLSFHeap
    //@―---------------------------------------------------------------------------
    struct TLSFHeap::BlockTag {
    public:

        BlockTag();
        BlockTag(byte* pData, s32 size);
        ~BlockTag();

        s32 GetDataSize();                                      // 運用データサイズを取得
        s32 GetAllSize();                                       // 自身が占有するすべてのデータサイズを取得
        BlockTag* Register(BlockTag* ndp);      // 双方向リストへ入れる
        BlockTag* Remove();                             // 双方向リストから取り除く

    public:

        bool m_flag;        //!< 使用フラグ
        s32 m_size;         //!< データサイズ
        byte* m_pData;      //!< データ部への先頭ポインタ
        s32* m_pAllSize;    //!< EndTagへのポインタ
        BlockTag* m_pPrev;
        BlockTag* m_pNext;
    };
    //! @endcond


    //===============================================================
    // インライン関数
    //===============================================================

    //@―---------------------------------------------------------------------------


}// namespcae ob
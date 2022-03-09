﻿//***********************************************************
//! @file
//! @brief		参照カウントオブジェクト
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {

    //@―---------------------------------------------------------------------------
    //! @brief      参照カウントオブジェクト
    //! 
    //! @details    参照カウントによるメモリ管理をする場合はこのクラスを継承してくだ
    //!             さい。
    //!             一般的には RefObject 用のスマートポインタである Get を使用して参
    //!             照を管理します。
    //@―---------------------------------------------------------------------------
    class OB_API RefObject :private Noncopyable {
        template<class T>
        friend class Ref;
        friend class RefObjectHelper;
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        RefObject();


        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        ~RefObject();


    private:

        //@―---------------------------------------------------------------------------
        //! @brief  参照カウントの取得
        //@―---------------------------------------------------------------------------
        s32 GetReferenceCount()const noexcept;


        //@―---------------------------------------------------------------------------
        //! @brief  参照の追加
        //@―---------------------------------------------------------------------------
        s32 Retain();


        //@―---------------------------------------------------------------------------
        //! @brief  参照の解放
        //@―---------------------------------------------------------------------------
        s32 Release();

    private:

        atomic<s32> m_referenceCount;

    };


    //@―---------------------------------------------------------------------------
    //! @brief  IRefObject の参照を直接操作するためのユーティリティ
    //! 
    //! @details    IRefObject を継承したクラスは通常 MakeRef 関数をによって生成され
    //!             Get クラスを通して操作されるが、特殊な用途において直接操作したい
    //!             場合にこのクラスを用いる。
    //@―---------------------------------------------------------------------------
    class OB_API RefObjectHelper {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  参照カウントの取得
        //! 
        //! @param pObj 捜査対象の IRefObject
        //@―---------------------------------------------------------------------------
        static s32 GetReferenceCount(RefObject* pObj);


        //@―---------------------------------------------------------------------------
        //! @brief  参照の追加
        //! 
        //! @param pObj 捜査対象の IRefObject
        //@―---------------------------------------------------------------------------
        static s32 Retain(RefObject* pObj);


        //@―---------------------------------------------------------------------------
        //! @brief  参照の解放
        //! 
        //! @param pObj 捜査対象の IRefObject
        //@―---------------------------------------------------------------------------
        static s32 Release(RefObject* pObj);


    };

}// namespcae ob
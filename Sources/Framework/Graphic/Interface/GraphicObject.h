﻿//***********************************************************
//! @file
//! @brief		グラフィック・オブジェクト
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      グラフィック・オブジェクト
    //! 
    //! @details    グラフィック・オブジェクトは描画時まで削除されない必要があります。
    //!             描画はフレーム内に収まらず数フレーム遅れる場合もあるため、
    //!             所有者の寿命で管理するのが難しくなします。
    //!             そこで、このクラスを継承するクラスは所有者のデストラクタで削除されずに
    //!             システムの遅延開放スタックに積まれます。
    //!             システムのスタックに積まれている間は破棄されていないことが保証できるため
    //!             GPUからのアクセスにも対応できます。
    //!```          
    //!             class ITexture:public GraphicObject{
    //!             }
    //! 
    //!             class Texture{
    //!             public:
    //!                 OB_DEFINE_GRAPHIC_OBJECT_HOLDER(Texture);
    //!             private:
    //!                 ITexture* m_pImpl;
    //!             }
    //! 
    //!             class System:public Singleton(System){
    //!             public:
    //!                 void stackDelayDelete(GraphicObject* pObject){
    //!                     m_delayDeleteStack.push_back(pObject);               
    //!                 }
    //!             }
    //! ```
    //@―---------------------------------------------------------------------------
    class GraphicObject {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        virtual ~GraphicObject();


        //@―---------------------------------------------------------------------------
        //! @brief      名前を取得
        //@―---------------------------------------------------------------------------
        const Char* getName()const;


        //@―---------------------------------------------------------------------------
        //! @brief      名前を設定
        //@―---------------------------------------------------------------------------
        void setName(StringView );


        //@―---------------------------------------------------------------------------
        //! @brief      参照を追加
        //@―---------------------------------------------------------------------------
        void addReference();


        //@―---------------------------------------------------------------------------
        //! @brief      参照を解放
        //@―---------------------------------------------------------------------------
        s32 releaseReference();


        //@―---------------------------------------------------------------------------
        //! @brief      参照カウントを取得
        //@―---------------------------------------------------------------------------
        s32 getReferenceCount()const;


    protected:

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //! 
        //! @param name オブジェクト名
        //@―---------------------------------------------------------------------------
        GraphicObject(StringView name=TC("NONAME"));


    private:

        GraphicObject*  m_pStack;    //!< 削除スタック用ポインタ
        atomic<s32>     m_referenceCount;

#ifdef OB_DEBUG
        Char      m_name[100];
#endif

    };

}// namespcae ob::graphic
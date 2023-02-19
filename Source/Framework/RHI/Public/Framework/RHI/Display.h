﻿//***********************************************************
//! @file
//! @brief		ディスプレイ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/DisplayDesc.h>

//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      ディスプレイ
    //! 
    //! @details    モニターに表示するバッファを複数のバッファから切り替えることで
    //!             表示のちらつきを解決する。
    //@―---------------------------------------------------------------------------
    class Display :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief  生成
        //@―---------------------------------------------------------------------------
        static Ref<Display> Create(const DisplayDesc& desc);

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  定義を取得
        //@―---------------------------------------------------------------------------
        virtual const DisplayDesc& getDesc()const noexcept = 0;


        //@―---------------------------------------------------------------------------
        //! @brief      更新
        //@―---------------------------------------------------------------------------
        virtual void update() = 0;


    protected:

        virtual ~Display() = default;

    };


}// namespace pb::rhi
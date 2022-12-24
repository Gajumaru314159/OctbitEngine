﻿//***********************************************************
//! @file
//! @brief		パイプラインステート
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Utility/GraphicObjectHolder.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  パイプラインステート
    //@―---------------------------------------------------------------------------
    class PipelineState {
        OB_DEFINE_GRAPHIC_OBJECT_HOLDER(PipelineState);
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        PipelineState();


        //@―---------------------------------------------------------------------------
        //! @brief  デストラクタ
        //@―---------------------------------------------------------------------------
        PipelineState(const PipelineStateDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief      定義を取得
        //@―---------------------------------------------------------------------------
        const PipelineStateDesc& getDesc()const;


    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob::rhi
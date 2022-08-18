//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rendering {


    struct MaterialPropertyDesc {
        String name;
        u32 offset;
    };

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    struct MaterialDesc{
        Array<MaterialPropertyDesc> floatProperties;
        Array<MaterialPropertyDesc> colorProperties;
        Array<MaterialPropertyDesc> matrixProperties;
        Array<MaterialPropertyDesc> textureProperties;
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob
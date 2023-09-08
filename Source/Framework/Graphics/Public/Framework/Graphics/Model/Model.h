//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/Types/SubPass.h>
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/MaterialDesc.h>

namespace ob::graphics {

    class Bone {

    };

    //@―---------------------------------------------------------------------------
    //! @brief      モデル
    //@―---------------------------------------------------------------------------
    class Model:public RefObject {
    public:

        Bone* findBone(StringView name)const;
        Ref<Material> findMaterial(StringView name)const;


        
    };

}
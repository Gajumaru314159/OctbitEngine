//***********************************************************
//! @file
//! @brief		シェーダ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/BlobView.h>
#include <Framework/RHI/Shader.h>
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/ShaderStage.h>

namespace ob::rhi {

    //! @brief      シェーダ
    class IShader :public GraphicObject {
    public:

        //! @brief          シェーダステージを取得
        virtual ShaderStage getStage()const = 0;

    };

}


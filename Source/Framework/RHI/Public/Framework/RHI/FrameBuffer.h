//***********************************************************
//! @file
//! @brief		ルートシグネチャ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/FrameBufferDesc.h>

namespace ob::rhi {

    //! @brief      フレームバッファ
    //! @details    描画ターゲットのコレクションです。
    class FrameBuffer :public GraphicObject {
    public:

        //! @brief  生成
        static Ref<FrameBuffer> Create(const FrameBufferDesc& desc);

    public:

    };


}
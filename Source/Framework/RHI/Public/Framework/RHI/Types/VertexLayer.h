//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/PipelineStateDesc.h>

namespace ob::rhi {

    class InputLayt {
    public:
        InputLayt& add(Type);
        InputLayt& addFloat1();
        InputLayt& addFloat2();
        InputLayt& addFloat3();
        InputLayt& addFloat4();
        InputLayt& addInt4();

        InputLayt& nextStream();

        VertexLayout create();
    private:
        Array<VertexAttribute> attributes;	//!< 属性リスト
    };

    //void test() {
    //    auto vertexLayout = 
    //        InputLayt()
    //        .addFloat4()    // Position
    //        .addFloat4()    // Normal
    //        .addFloat4()    // Color
    //        .nextStream()
    //        .addFloat1()    // Time
    //        .create();
    //}

}// namespcae ob
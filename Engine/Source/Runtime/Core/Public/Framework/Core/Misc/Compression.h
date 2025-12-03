//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CoreTypes.h>

namespace ob::core {

    //! @brief  圧縮
    class Compression {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //! @brief  説明

    private:



    };

    class GDeflate {
    public:
        static constexpr u32 MinCompressionLevel = 1;
        static constexpr u32 MaxCompressionLevel = 12;
    public:
        static bool Compress(u8* output,size_t* outputSize,const u8* in,size_t inSize,u32 level,u32 flags);
        static bool Decompress(u8* output, size_t outputSize, const u8* in, size_t inSize, u32 numWorkers);
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}
//***********************************************************
//! @file
//! @brief		CRCハッシュ関数
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Misc/Compression.h>
#include <GDeflate.h>

namespace ob::core {

    bool GDeflate::Compress(u8* output, size_t* outputSize, const u8* in, size_t inSize, u32 level, u32 flags) {
		return ::GDeflate::Compress(output, outputSize, in, inSize, level, flags);
    }
    bool GDeflate::Decompress(u8* output, size_t outputSize, const u8* in, size_t inSize, u32 numWorkers) {
		return ::GDeflate::Decompress(output, outputSize, in, inSize, numWorkers);
    }

} 

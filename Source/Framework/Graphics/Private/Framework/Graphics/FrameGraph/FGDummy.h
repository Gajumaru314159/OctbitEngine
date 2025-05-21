//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphics {

    //! @brief      FrameGraphダミーインスタンス
    class FGDummy{
    public:
        using Desc = int;

        void create(const Desc& desc, void* allocator) {}
        void destroy(const Desc& desc, void* allocator) {}
        static std::string toString(const Desc& desc) { return "dummy"; }
    };

}
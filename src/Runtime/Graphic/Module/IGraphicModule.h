//***********************************************************
//! @file
//! @brief		グラフィック・モジュール
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Core/Module/IModule.hpp>
#include <Graphic/Device/Device.hpp>

namespace ob {
    namespace graphic {

        enum class FeatureLevel {
            Default,
            ES2_REMOVED,
            ES3_1,
            SM4_REMOVED,
            SM5,
            Max
        };

        //-----------------------------------------------------------
        //! @brief      グラフィック・モジュール
        //! 
        //! @details    グラフィックAPIのモジュール基底です。
        //!             DirectXやVulkanなど、APIを共通化する場合はこのモジュール・インターフェースを実装して下さい。
        //-----------------------------------------------------------
        class IGraphicModule :public IModule     {
        public:

            virtual bool IsSupported() = 0;
            virtual graphic::Device CreateDevice(FeatureLevel featureLevel=FeatureLevel::Default) = 0;

        private:



        };






        //===============================================================
        // インライン関数
        //===============================================================

        //-----------------------------------------------------------


    }// namespace graphic
}// namespcae ob
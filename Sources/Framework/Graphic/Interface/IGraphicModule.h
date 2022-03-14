//***********************************************************
//! @file
//! @brief		グラフィック・モジュール
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Platform/Module/IModule.h>
#include <Framework/Graphic/Types/FeatureLevel.h>

namespace ob::graphic {
    class IDevice;
}

namespace ob::graphic {

    //-----------------------------------------------------------
    //! @brief      グラフィック・モジュール
    //! 
    //! @details    グラフィックAPIのモジュール基底です。
    //!             DirectXやVulkanなど、APIを共通化する場合はこのモジュール・インターフェースを実装して下さい。
    //-----------------------------------------------------------
    class IGraphicModule :public platform::IModule {
    public:

        virtual bool isSupported() = 0;
        virtual IDevice* createDevice(FeatureLevel featureLevel = FeatureLevel::Default) = 0;

        //@―---------------------------------------------------------------------------
        //! @brief マジックコードを取得
        //@―---------------------------------------------------------------------------
        u32 magicCode()const override final { return graphicMagicCode(); }


        static constexpr u32 graphicMagicCode() {
            return 132582935;
        }

    protected:


    private:



    };






    //===============================================================
    // インライン関数
    //===============================================================


}// namespace pb::graphic
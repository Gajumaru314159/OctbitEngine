//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "Entry.h"
#include <Graphic/Module/IGraphicModule.h>
#include <d3d12.h>

class GraphicD3D12Module :public ob::graphic::IGraphicModule {
public:
    GraphicD3D12Module() {

    }
    
    ~GraphicD3D12Module() {

    }


    void Startup() override{
        // PIX開始
    }
    void Shutdown() override {
        // PIX終了
    }


    bool IsSupported() override{
        return true;
    };

    ob::graphic::D3D12Device CreateDevice(ob::graphic::FeatureLevel featureLevel)override {
        
        
        //D3D12CreateDevice();
    }
};

ob::IModule* GetModule() {
    static GraphicD3D12Module test;
    return &test;
}
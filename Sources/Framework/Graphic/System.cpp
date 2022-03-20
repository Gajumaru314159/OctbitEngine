//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/System.h>
#include <Framework/Platform/Module/ModuleManager.h>
#include <Framework/Graphic/Interface/IGraphicModule.h>
#include <Framework/Graphic/Interface/IDevice.h>
#include <Framework/Graphic/Interface/GraphicObject.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    System::System(GraphicAPI api)
        : m_nowStackIndex(0){
        ref();

        IDevice* pDevice = nullptr;
        if (api == GraphicAPI::D3D12) {
#if defined(OS_WINDOWS)
            if (auto pModule = platform::ModuleManager::ref().loadModule<IGraphicModule>(TC("D:/My/Productions/C++/OctbitEngine/build/Sources/Plugins/GraphicDirectX12/Debug/GraphicDirectX12"))) {

                if (pModule->magicCode() == IGraphicModule::graphicMagicCode()) {
                    pDevice = pModule->createDevice(FeatureLevel::Default);
                }

            }
            
#endif
        } else if (api == GraphicAPI::Vulkan) {
            OB_NOTIMPLEMENTED();
        }


        m_device.m_impl = pDevice;
        OB_CHECK_ASSERT_EX(pDevice,"無効なグラフィックモジュール[GraphicAPI={0}]",api);

        // とりあえず固定で3フレーム
        m_delayReleaseStack.resize(3);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    System::~System() {
        for (s32 i = 0; i < m_delayReleaseStack.size(); ++i) {
            update();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デバイスを取得
    //@―---------------------------------------------------------------------------
    Device& System::getDevice() {
        return m_device;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      デバイスを取得デバイスを取得
    //@―---------------------------------------------------------------------------
    void System::requestRelease(GraphicObject& pObject) {
        if (m_delayReleaseStack.empty()) {
            delete &pObject;
        } else {
            auto& nowStack = m_delayReleaseStack[m_nowStackIndex];
            nowStack.emplace(&pObject);
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief      更新
    //@―---------------------------------------------------------------------------
    void System::update() {

        // 削除スタックのインデックスを更新
        m_nowStackIndex = (m_nowStackIndex+1)%get_size(m_delayReleaseStack);

        // グラフィック・オブジェクトを削除
        auto& deleteStack = m_delayReleaseStack[m_nowStackIndex];
        while (!deleteStack.empty()) {
            auto pObject = deleteStack.top();
            deleteStack.pop();

            delete pObject;
        }
    }

}// namespace pb::graphic
//***********************************************************
//! @file
//! @brief		システム
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/System.h>

#include <Framework/Platform/Module/ModuleManager.h>
#include <Framework/Graphic/GraphicObjectManager.h>

#include <Framework/Graphic/Interface/IGraphicModule.h>
#include <Framework/Graphic/Interface/IDevice.h>
#include <Framework/Graphic/Interface/GraphicObject.h>
#include <Framework/Core/String/StringEncoder.h>

#include <magic_enum.hpp>

namespace ob::graphic {

    System::System() = default;
    System::~System() = default;

    //@―---------------------------------------------------------------------------
    //! @brief          初期化
    //! 
    //! @details        プラットフォームにより使用できないAPIがあります。
    //!                 使用できるプラットフォームは GraphicAPI を参照してください。
    //!                 システムの初期化に失敗した場合アサートを発生させます。
    //! @param desc     使用するグラフィックAPIタイプ
    //! @retval true    成功
    //! @retval false   失敗
    //@―---------------------------------------------------------------------------
    bool System::initialize(SystemDesc desc) {

        if (m_device) {
            LOG_FATAL_EX("Graphic", "グラフィック・システムは初期化済みです。");
            return false;
        }

        IDevice* pDevice = nullptr;

        if (desc.api == GraphicAPI::D3D12) {

#if defined(OS_WINDOWS)
            if (auto pModule = platform::ModuleManager::Instance().loadModule<IGraphicModule>(TC("GraphicDirectX12"))) {

                if (pModule->magicCode() == IGraphicModule::graphicMagicCode()) {
                    pDevice = pModule->createDevice(FeatureLevel::Default);
                }

            }
#endif

        } else if (desc.api == GraphicAPI::Vulkan) {

        }

        if (pDevice == nullptr) {
            auto rawName = magic_enum::enum_name(desc.api);
            String name;
            StringEncoder::Encode(rawName, name);

            LOG_ERROR_EX("Graphic","無効なグラフィックモジュール[GraphicAPI={0}]", name.c_str());
            return false;
        }

        if (!pDevice->isValid()) {
            LOG_ERROR_EX("Graphic", "グラフィックデバイスの初期化に失敗");
            delete pDevice;
            return false;
        }

        m_device = UPtr<IDevice>(pDevice);
        m_objectManager = std::make_unique<GraphicObjectManager>(desc.bufferCount);
        return true;
    }

    //@―---------------------------------------------------------------------------
    //! @brief      更新
    //@―---------------------------------------------------------------------------
    void System::update() {
        if (m_device) {
            m_device->update();
        }
        if (m_objectManager) {
            m_objectManager->update();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief      サポートしているAPIか
    //@―---------------------------------------------------------------------------
    bool System::IsSupported(GraphicAPI api) {
        auto apis = GetSupportedApiList();
        return apis.find(api) != apis.end();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      グラフィックAPIがサポートされているか
    //@―---------------------------------------------------------------------------
    Set<GraphicAPI> System::GetSupportedApiList() {

        Set<GraphicAPI> apis;

#ifdef OS_WINDOWS
        apis.emplace(GraphicAPI::D3D12);
        apis.emplace(GraphicAPI::Vulkan);
#endif // OS_WINDOWS


        return move(apis);
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デバイスを取得
    //@―---------------------------------------------------------------------------
    IDevice* System::getDevice() {
        OB_CHECK_ASSERT_EXPR(m_device);
        return m_device.get();
    }


    //@―---------------------------------------------------------------------------
    //! @brief  グラフィック・オブジェクトを解放
    //@―---------------------------------------------------------------------------
    void System::registerObject(GraphicObject& object) {

        if (!m_objectManager) {
            LOG_FATAL_EX("Graphic", "システムの開放後にグラフィックオブジェクトを解放しようとしました。");
            return;
        }

        m_objectManager->registerObject(object);

    }


    //@―---------------------------------------------------------------------------
    //! @brief  グラフィック・オブジェクトを解放
    //@―---------------------------------------------------------------------------
    void System::requestRelease(GraphicObject& object) {

        if (!m_objectManager) {
            LOG_FATAL_EX("Graphic", "システムの開放後にグラフィックオブジェクトを解放しようとしました。");
            return;
        }

        m_objectManager->requestRelease(object);

    }

}// namespace pb::graphic
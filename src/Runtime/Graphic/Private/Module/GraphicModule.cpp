//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Runtime/Graphic/Private/Module/GraphicModule.h>
//#include <Runtime/GraphicD3D12/Private/Module/GraphicModule.h>

namespace ob {
    namespace graphic {

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        GraphicModule::GraphicModule(GraphicAPI graphicAPI) {
            switch (graphicAPI) {
            case GraphicAPI::D3D12:
                m_module = make_unique<D3D12GraphicModule>();
                break;
            default:
                OB_NOTIMPLEMENTED();
                break;
            }

            OB_ENSURE(m_module.get());
        }

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        Ref<ITexture> GraphicModule::CreateTexture() {
            ITexture* pTexture = m_module->CreateDevice();
            return Ref<ITexture>(pTexture);
        }

    }
}// namespace ob
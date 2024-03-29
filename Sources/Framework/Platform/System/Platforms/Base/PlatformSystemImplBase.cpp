﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "PlatformSystemImplBase.h"

namespace ob::platform {
    namespace detail {

        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        void PlatformSystemImplBase::createMainWindow() {
            WindowCreationDesc desc;
            desc.fullscreen = false;
            desc.resizable = true;
            desc.clientSize.width = 1280;
            desc.clientSize.height = 720;
            m_mainWindow = MakeRef<Window>(desc);
        }


        //@―---------------------------------------------------------------------------
        //! @brief  説明
        //@―---------------------------------------------------------------------------
        Ref<Window>& PlatformSystemImplBase::getMainWindow() {
            return m_mainWindow;
        }


    }// namespace detail
}// namespace ob::platform

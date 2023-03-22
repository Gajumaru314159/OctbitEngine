//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Camera.h>

namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      カメラ
    //! @details    CameraCompoentとは異なりReflectionCaptureやShadowCaptureでも
    //!             使用されます。
    //@―---------------------------------------------------------------------------
    class CameraManager {
    public:
        static CameraManager& Get();
    public:

        CameraManager() {

        }

        Camera* getMainCamera() {
            ScopeLock lock(m_lock);
            if (m_cameras.empty())return nullptr;
            return m_cameras.front();
        }

        void add(Camera* camera) {
            ScopeLock lock(m_lock);
            m_cameras.push_back(camera);
        }
        void remove(Camera* camera) {
            ScopeLock lock(m_lock);
            m_cameras.remove(camera);
        }

    private:

        SpinLock        m_lock;
        List<Camera*>   m_cameras;

    };

}// namespcae ob
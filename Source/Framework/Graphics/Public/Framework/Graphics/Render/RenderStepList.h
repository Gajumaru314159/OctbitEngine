//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>

namespace ob::graphics {

	//@―---------------------------------------------------------------------------
	//! @brief      RenderStepの追加のみを許可するラッパー
	//@―---------------------------------------------------------------------------
    class RenderStepList {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      コンストラクタ
        //@―---------------------------------------------------------------------------
        RenderStepList(Array<UPtr<RenderStep>>& container)
            : m_container(container) 
        {
        }

        //@―---------------------------------------------------------------------------
        //! @brief      RenderStepを追加
        //@―---------------------------------------------------------------------------
        template<class T,class... Args>
        void add(Args&&... args) {
            m_container.emplace_back(std::make_unique<T>(args...));
        }

    private:
        Array<UPtr<RenderStep>>& m_container;
    };

}
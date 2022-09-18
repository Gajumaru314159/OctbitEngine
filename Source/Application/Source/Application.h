//***********************************************************
//! @file
//! @brief		アプリケーションテスト
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/App.h>
#include <Framework/Engine/ModuleManager.h>
#include <Framework/Platform/DLL.h>
#include <Framework/Graphic/Interface/IGraphicModule.h>

class App :public ob::engine::AppBase {
public:
	App()
		: m_dll("GraphicDirectX12")
	{
	}

private:

	void onModuleSetup(ob::engine::ModuleManager& manager) {
		if (auto func = m_dll.getFunction(TC("RegisterModule"))) {
			func.call<void,ob::engine::ModuleManager&>(manager);
		}

		LOG_INFO("Success");

		//if (m_dll.tryCall(TC("RegisterModule"), manager)) {
		//}
		manager.create<ob::graphic::IGraphicModule2>();

	}

private:

	ob::platform::DLL m_dll;

};
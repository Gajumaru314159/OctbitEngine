//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#ifdef OB_DEBUG
#include "PIXModule.h"
#include <Framework/Engine/Engine.h>
#include <Framework/RHI/Config.h>
#include <filesystem>
#include <shlobj.h>

namespace {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//! @link https://devblogs.microsoft.com/pix/taking-a-capture/
	//@―---------------------------------------------------------------------------
	static std::wstring GetLatestWinPixGpuCapturerPath_Cpp17()
	{
		LPWSTR programFilesPath = nullptr;
		SHGetKnownFolderPath(FOLDERID_ProgramFiles, KF_FLAG_DEFAULT, NULL, &programFilesPath);

		std::filesystem::path pixInstallationPath = programFilesPath;
		pixInstallationPath /= "Microsoft PIX";

		std::wstring newestVersionFound;
		if (std::filesystem::exists(pixInstallationPath)) {
			for (auto const& directory_entry : std::filesystem::directory_iterator(pixInstallationPath))
			{
				if (directory_entry.is_directory())
				{
					if (newestVersionFound.empty() || newestVersionFound < directory_entry.path().filename().c_str())
					{
						newestVersionFound = directory_entry.path().filename().c_str();
					}
				}
			}
		}

		return pixInstallationPath / newestVersionFound / L"WinPixGpuCapturer.dll";
	}

}


namespace ob::rhi::dx12 {

	//@―---------------------------------------------------------------------------
	//! @brief  説明
	//@―---------------------------------------------------------------------------

	PIXModule::PIXModule() 
		: m_hModule(nullptr)
	{
		//if (GEngine->config().get<Config>().enablePIX == false)
		//	return;

		LOG_INFO("PIXを有効化");

		auto pixPath = GetLatestWinPixGpuCapturerPath_Cpp17();

		if (GetModuleHandleW((LPCWSTR)L"WinPixGpuCapture.dll") != 0) {
			goto ERROR_END;
		}
		if (!pixPath.empty()) {
			LOG_TRACE_EX("Graphic", "PIXを起動");
			m_hModule = LoadLibraryW(pixPath.c_str());
		}
		if (m_hModule == nullptr) {
			goto ERROR_END;
		}

		return;
	ERROR_END:
		release();
	}
	PIXModule::~PIXModule() {
		release();
	}

	void PIXModule::release() {
		if (m_hModule) {
			LOG_TRACE_EX("Graphic", "PIXを終了");
			::FreeLibrary(m_hModule);
		}
		m_hModule = nullptr;
	}

}// namespace ob

#endif// OB_DEBUG
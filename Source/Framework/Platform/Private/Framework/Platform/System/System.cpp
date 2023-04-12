//***********************************************************
//! @file
//! @brief		プラットフォーム・システム
//! @author		Gajumaru
//***********************************************************
#ifdef OS_WINDOWS
#include <Framework/Platform/System.h>
#include <Framework/Platform/Window/WindowManager.h>
#include <Framework/Core/Utility/DI.h>
#include <Framework/Core/Platform/WindowsHeaders.h>

namespace ob::platform {

	//@―---------------------------------------------------------------------------
	//! @brief      システムをServiceInjectorに登録
	//@―---------------------------------------------------------------------------
	void Register(ServiceInjector& injector) {
		injector.bind<WindowManager>();
	}

}

namespace ob::platform::System {

	//@―---------------------------------------------------------------------------
	//! @brief  プラットフォームの言語を取得
	//@―---------------------------------------------------------------------------
	Language GetLanguage() {
		LANGID langID = GetSystemDefaultLangID();
		switch (langID) {
		case 0x0411:
			return Language::Japanese;
		case 0x0C09:
		case 0x2809:
		case 0x1009:
		case 0x0809:
		case 0x1809:
		case 0x2009:
		case 0x1409:
		case 0x2C09:
		case 0x0409:
		case 0x1C09:
			return Language::English;
		case 0x040C:
		case 0x080C:
		case 0x0C0C:
		case 0x100C:
		case 0x140C:
			return Language::French;
		case 0x0410:
		case 0x0810:
			return Language::Italian;
		case 0x0407:
		case 0x0C07:
		case 0x0807:
		case 0x1407:
		case 0x1007:
			return Language::German;
		case 0x040A:
		case 0x2C0A:
		case 0x400A:
		case 0x340A:
		case 0x240A:
		case 0x140A:
		case 0x1C0A:
		case 0x300A:
		case 0x100A:
		case 0x480A:
		case 0x080A:
		case 0x4C0A:
		case 0x180A:
		case 0x280A:
		case 0x500A:
		case 0x3C0A:
		case 0x440A:
		case 0x380A:
		case 0x200A:
		case 0x0000:
			return Language::Spanish;
		case 0x0412:
			return Language::Korean;
		case 0x0816:
		case 0x0416:
			return Language::Portuguese;
		case 0x0419:
		case 0x0819:
			return Language::Russian;
		case 0x0804:
		case 0x0C04:
		case 0x1004:
		case 0x0404:
			return Language::Chinese;
		}
		return Language::Unknown;
	}

}
#endif
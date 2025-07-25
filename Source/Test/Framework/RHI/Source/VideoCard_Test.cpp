//***********************************************************
//! @file
//! @brief		Buffer のテスト
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>
#include <magic_enum.hpp>

using namespace ob;
using namespace ob::rhi;

TYPED_TEST(RHITest, VideoCard_Get) {

	auto videoCards = Device::Get()->getVideoCards();

	for (auto& videoCard : videoCards) {
		ASSERT_NE(videoCard.name, "");
		// ASSERT_NE(videoCard.memory, 0);
		// ASSERT_NE(videoCard.outputs.size(), 0);
		for (auto& output : videoCard.outputs) {
			ASSERT_NE(output.name, "");
			// ASSERT_NE(output.modes.size(), 0);
			for (auto& mode : output.modes) {
				ASSERT_NE(mode.width, 0);
				ASSERT_NE(mode.height, 0);
				ASSERT_NE(mode.refreshRate, 0);
				ASSERT_NE(mode.format, TextureFormat::Unknown);
			}
		}
	}

}
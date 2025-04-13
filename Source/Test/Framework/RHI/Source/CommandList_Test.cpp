//***********************************************************
//! @file
//! @brief		Buffer のテスト
//! @author		Gajumaru
//***********************************************************
#include <RHITestBase.h>
#include <magic_enum.hpp>

class CommandListTest : public RHITestBase {};

TEST_F(CommandListTest, Create) {

    for (auto type : magic_enum::enum_values<CommandListType>()) {

        CommandListDesc desc;
        desc.type = type;

        auto commandList = CommandList::Create(desc);

        ASSERT_NE(commandList, nullptr);

    }

}

TEST_F(CommandListTest, GraphicCommands) {
    return;

    RenderTextureDesc rtDesc;
	rtDesc.size = { 1280, 720 };
    auto target = RenderTexture::Create(rtDesc);


    CommandListDesc desc;
    desc.type = CommandListType::Graphic;

    auto commandList = CommandList::Create(desc);

    ASSERT_NE(commandList, nullptr);

    commandList->begin();

    Viewport viewport;
    viewport.left = 0;
    viewport.top = 0;
    viewport.right = 1280;
    viewport.bottom = 720;
    commandList->setViewport(&viewport, 1);

    IntRect scissor;
    scissor.left = 0;
    scissor.top = 0;
    scissor.right = 1280;
    scissor.bottom = 720;
    commandList->setScissorRect(&scissor, 1);

	commandList->setRenderTarget(target);

    //commandList->clearColors();

    commandList->pushMarker("Marker");
    commandList->popMarker();


    commandList->end();
    commandList->flush();
}
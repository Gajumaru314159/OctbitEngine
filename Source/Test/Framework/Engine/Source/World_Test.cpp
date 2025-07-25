//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Engine/All.h>

using namespace ob;
using namespace ob::engine;

TEST(Engine, Create) {

	auto world = World::Create("Main");
	world->getRootScene().addEntity(
		Entity::Create("Test")
	);

}
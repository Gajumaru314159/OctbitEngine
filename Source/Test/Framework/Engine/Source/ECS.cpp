//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Core/Reflection/TypeBuilder.h>
#include <Framework/Engine/ECS.h>

using namespace ob;
using namespace ob::engine2;


OB_DEFINE_CLASS_INFO(ob::engine2::TransformComponent) {
	constructor();
	field("local", &T::local);
	field("world", &T::world);
}
OB_DEFINE_CLASS_INFO(ob::engine2::RigidbodyComponent) {
	constructor();
	field("velocity", &T::velocity);
}



TEST(ECS, Construct) {
	TypeInfoManager manager;

	ECS ecs;

	auto entity0 = ecs.create<TransformComponent>();
	auto entity1 = ecs.create<TransformComponent, RigidbodyComponent>();
	auto entity2 = ecs.create("ob::engine2::TransformComponent,ob::engine2::RigidbodyComponent");

	auto& trans = ecs.get<TransformComponent>(entity0);
	trans.local.position.set(1, 2, 3);
	ecs.destroy(entity0);
	entity0 = ecs.create<TransformComponent>();
	auto& trans2 = ecs.get<TransformComponent>(entity0);


	auto& trans3 = ecs.get("ob::engine2::TransformComponent", entity0).as<TransformComponent>();

	ecs.destroy(entity1);

	Func<void(TransformComponent&)> func = 
		[](TransformComponent& t) {
			t.local.position.x += 1;
		};
	ecs.update2(func);

}
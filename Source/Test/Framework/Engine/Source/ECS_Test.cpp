//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/TypeInfoManager.h>
#include <Framework/Core/Reflection/TypeBuilder.h>
#include <Framework/Engine/ECS.h>

using namespace ob;
using namespace ob::engine2;


struct TransformComponent : Component {
	Transform local;
	Transform world;
	Entity    parent;
};
struct RigidbodyComponent : Component {
	Vec3 velocity;
};

class RigidbodySystem {
public:
	static void Update(TransformComponent& transform, RigidbodyComponent& rigidbody) {
		transform.local.position += rigidbody.velocity;
	}
};

class TransformSystem {
public:
	static void Update(TransformComponent& transform) {
		ECS ecs;



		transform.parent;
		transform.world = transform.local;
	}
};
class TransformStartupSystem {
public:
	static void Update(TransformComponent& transform) {
	}
};


OB_DEFINE_CLASS_INFO(ob::engine2::Component) {
}
OB_DEFINE_CLASS_INFO(TransformComponent) {
	base<Component>();
	constructor();
	field("local", &T::local);
	field("world", &T::world);
}
OB_DEFINE_CLASS_INFO(RigidbodyComponent) {
	base<Component>();
	constructor();
	field("velocity", &T::velocity);
}



TEST(ECS, Construct) {
	TypeInfoManager manager;

	ECS ecs;

	auto entity0 = ecs.create<TransformComponent>();
	auto entity1 = ecs.create<TransformComponent, RigidbodyComponent>();
	auto entity2 = ecs.create("TransformComponent,RigidbodyComponent");

	auto& trans = ecs.get<TransformComponent>(entity0);
	trans.local.position.set(1, 2, 3);
	ecs.destroy(entity0);
	entity0 = ecs.create<TransformComponent>();
	ecs.get<TransformComponent>(entity0);


	ecs.get("TransformComponent", entity0).as<TransformComponent>();

	ecs.destroy(entity1);

	s32 count = 0;

	Func<void(TransformComponent&,RigidbodyComponent&)> func = 
		[&](TransformComponent& t, RigidbodyComponent&) {
			t.local.position.x += 1;
			count++;
		};

	ecs.update(func);
	ecs.update<TransformSystem>();
	ecs.update(TransformSystem::Update);

}


TEST(ECS, Many) {
	TypeInfoManager manager;

	ECS ecs;
	Entity entity;
	for (s32 i = 0; i < 10000; ++i) {
		ecs.create<TransformComponent>();
		entity = ecs.create<TransformComponent, RigidbodyComponent>();
		ecs.create("RigidbodyComponent");
	}
	EntityHandle handle(ecs,entity);
	{
		s32 count = 0;
		Func<void(TransformComponent&)> func = [&](auto&) { count++; };
		ecs.update(func);
		EXPECT_EQ(count, 20000);
	}

	{
		s32 count = 0;
		Func<void(RigidbodyComponent&)> func = [&](auto&) { count++; };
		ecs.update(func);
		EXPECT_EQ(count, 20000);
	}

	{
		s32 count = 0;
		Func<void(TransformComponent&,RigidbodyComponent&)> func = [&](auto&,auto&) { count++; };
		ecs.update(func);
		EXPECT_EQ(count, 10000);
	}
}

TEST(ECS, Iterator) {
	TypeInfoManager manager;

	ECS ecs;

	//for (auto [entity, transform] : ecs.view<TransformComponent>()) {
	//	transform.local.position.set(1, 2, 3);
	//}	

}

/*
TEST(ECS, Map) {

	TypeInfoManager manager;

	const auto T = Archetype::Create<TransformComponent>();
	const auto TR = Archetype::Create<TransformComponent, RigidbodyComponent>();
	const auto R = Archetype::Create<RigidbodyComponent>();

	ECS ecs;
	Entity entity = ecs.create<TransformComponent>();

	EXPECT_EQ(ecs.archetype(entity), T);

	entity = ecs.map<TransformComponent, RigidbodyComponent>(entity);

	EXPECT_EQ(ecs.archetype(entity), TR);

	entity = ecs.map<RigidbodyComponent>(entity);

	EXPECT_EQ(ecs.archetype(entity), R);

}
*/
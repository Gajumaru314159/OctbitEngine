//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#if 0
namespace ob::engine2 {

	struct Entity {
		u32 index;
		u16 archetype;
		u16 version;
	};

	struct TransformComponent {
		Transform local;
		Transform world;
		Entity    parent;
	};
	struct ModelComponent {
		Transform local;
		Transform world;
		Entity    parent;
	};

	struct RigidbodyComponent {
		Vec3 velocity;
	};

	struct CollisionComponent {

	};

	struct CameraComponent {
		f32 fov;
		Range clip;
	};

	struct LigthtComponent {
		s32 type;
		Color color;
		f32 intensity;
		f32 range;
	};

	struct BehaviorComponent {

	};


	class Archetype {
	public:
		template<class... TComponents>
		static s32 Index(bool readonly = true) {
			static s32 index = GenerateIndex(readonly);
			return index;
		}
	private:
		static s32 GenerateIndex(bool readonly) {
			OB_ASSERT(readonly, "読み取り専用のタイミングでアクセスされました。");
			static s32 index = 0;
			return index++;
		}
	};

	class Chunk {
	public:
		template<class T>
		T& get(u32 index) {
			return static_cast<T*>(data[index]);
		}

		template<class... TComponents>
		s32 create() {
			return 0;
		}
	};

	class ECS {
	public:

		template<class... TComponents>
		void reserve() {
			auto archetype = Archetype::Index<TComponents...>();
			for (auto& type : { Type::Get<TComponents>(),... }) {
				m_availables[type].emplate(archetype);
			}
		}

		template<class... TComponents>
		Entity create() {

			auto archetype = Archetype::Index<TComponents...>();
			
			auto index = m_chunks[archetype]->create<TComponents...>();

			Entity entity;
			entity.index = index;
			entity.archetype = archetype;
			entity.version = 0;

			return entity;
		}

		void destroy(Entity entity) {
			//m_chunks[entity.archetype]->destroy(entity.index);
		}

		template<class TComponent>
		TComponent& get(Entity entity) {
			m_chunks[entity.archetype]->get<TComponent>(entity.index);
		}

		template<class TSystem>
		void update() {
		}

		template<class... TComponents>
		void update2(Func<void(TComponents&...)> func) {
			// 全てのComponentのArchetypeの論理積を取る
			
			Vector<s32> archetypes;
			for (auto archetype : archetypes) {

				Entity entity;
				entity.archetype = archetype;

				for (s32 i = 0; i < 10; ++i) {
					entity.index = i;
					func(get<TComponents>(entity)...);
				}
			}


		}

		void merge(ECS& ecs) {
			// 他のECSのComponentを自分のECSに移す
		}
	private:
		HashMap<Type, HashSet<s32>> m_availables;
		Vector<UPtr<Chunk>> m_chunks;
	};


	class RigidbodySystem {
	public:
		static void Update(TransformComponent& transform,RigidbodyComponent& rigidbody) {
			transform.local.position += rigidbody.velocity ;
		}
	};

	class TransformSystem {
	public:
		static void Update(TransformComponent& transform) {
			ECS ecs;

			auto& parent = ecs.get<TransformComponent>(transform.parent).world;


			transform.parent;
			transform.world = parent * transform.local;
		}
	};

	void sample() {

		ECS ecs;

		auto entity0 = ecs.create<TransformComponent>();
		auto entity1 = ecs.create<TransformComponent,RigidbodyComponent>();
		auto entity2 = ecs.create<TransformComponent, CameraComponent>();
		auto entity3 = ecs.create<TransformComponent, ModelComponent>();
		auto entity4 = ecs.create<TransformComponent, LigthtComponent>();

		ecs.destroy(entity1);
		ecs.get<TransformComponent>(entity0);


		ecs.update<RigidbodySystem>();
		ecs.update<TransformSystem>();

		{
			ECS ecs2;
			auto entity5 = ecs.create<TransformComponent>();

			ecs.merge(ecs2);
		}




		Func<void(TransformComponent&)> f = TransformSystem::Update;
		ecs.update2(f);


	}

}

#endif
//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Any.h>
#include <Framework/Core/Reflection/TypeInfo.h>
#include <Framework/Core/String/StringSplitView.h>


#if false
namespace ob::engine2 {



	// デフォルトのテンプレート定義
	template<typename T, int N, typename... Types>
	struct TypeIndex;

	// 再帰的に型とインデックスをチェックするための部分特殊化
	template<typename T, int N, typename First, typename... Rest>
	struct TypeIndex<T, N, First, Rest...> {
		static constexpr int value = std::is_same<T, First>::value ?
			(N == 0 ? 0 : (TypeIndex<T, N - 1, Rest...>::value == -1 ? -1 : 1 + TypeIndex<T, N - 1, Rest...>::value)) :
			(TypeIndex<T, N, Rest...>::value == -1 ? -1 : 1 + TypeIndex<T, N, Rest...>::value);
	};



	struct Entity {
		u32 index;
		u16 archetype;
		u16 version;
	};


	class Archetype {
	public:
		template<class... TComponents>
		Archetype Create() {
			return { {Type::Get<TComponents>()...} , Index<TComponents...>() };
		}
	public:

		Archetype(StringView types) {
			for (auto type : CommaSplitView(types)) {
				if (auto info = TypeInfo::Find(type)) {
					m_types.push_back(info->type);
				}
			}
		}

		s32 index() const {
			return m_index;
		}
	private:

		template<class... TComponents>
		static s32 Index(bool readonly = true) {
			static s32 index = GenerateIndex(readonly);
			return index;
		}

		static s32 GenerateIndex(bool readonly) {
			OB_ASSERT(readonly, "読み取り専用のタイミングでアクセスされました。");
			static s32 index = 0;
			return index++;
		}
	private:
		FixedVector<Type, 8> m_types;
		s32 m_index;
	};


	class AnyVector : Noncopyable {
	public:
		AnyVector(Type type) {
			m_info = TypeInfo::Find(type);
			OB_ASSERT(m_info, "{}をリフレクション登録してください", type.name());
			m_constructor = m_info->findConstructor();
			OB_ASSERT(m_info, "{}にデフォルトコンストラクタを追加してください", type.name());

			m_size = 0;
			m_capacity = 16 * 1024 / m_info->stride();
			m_blob.resize(m_capacity * m_info->stride());
		}
		void push_back() {
			auto ptr = GetOffsetPtr(m_blob.data(), m_info->stride() * m_size);
			m_info->findConstructor();
			m_constructor->placedInvoker(ptr, {});
			m_size++;
		}
		size_t size() const {
			return m_size;
		}
		size_t capacity() const {
			return m_size;
		}
	private:
		const TypeInfo* m_info;
		const ConstructorInfo* m_constructor;
		Blob m_blob;
		size_t m_size;
		size_t m_capacity;
	};


	class ComponentAllocator {
	public:
		ComponentAllocator(Type type)
		{
			m_info = TypeInfo::Find(type);
			OB_ASSERT(m_info, "{}をリフレクション登録してください",type.name());
			m_chunks.emplace_back(m_info->stride());
		}

		void push_back() {

			auto* chunk = &m_chunks.back();

			// 現在のチャンクがいっぱいの場合新しいチャンクを生成
			if (chunk->size() + 1 == chunk->capacity()) {
				m_chunks.emplace_back(type);
				chunk = &m_chunks.emplace_back(m_info->stride());
				chunk->reserve(m_span);
			}

			auto index = size();

			// チャンクに要素を追加
			chunk->emplace_back();

			return index;
		}

		s32 size() const {
			return (m_chunks.size() - 1) * m_span + m_chunks.back().size();
		}

		T& at(size_t index) {
			return m_chunks.at(index / m_span).at(index % m_span);
		}
		const T& at(size_t index) const {
			return m_chunks.at(index / m_span).at(index % m_span);
		}
	private:
		Vector<AnyVector> m_chunks;
	};



	class Chunk {
	public:
		Chunk(Span<Type> types) {
			for (auto type : types) {
				if (auto info = TypeInfo::Find(type)) {
					auto& chunk = m_chunks[info->type];
					chunk.info = info;

				} else {
					throw Exception(Format(("{}をリフレクション登録してください", type.name())));
				}
			}
		}

		template<class T>
		T& get(u32 index) {
			return get(Type::Get<T>(), index);
		}

		Any get(Type type, u32 index) {
			auto itr = m_chunks.find(type);
			if (itr != m_chunks.end()) return {};

			auto& chunk = itr->second;

			s32 stride = chunk.info->stride;
			
			Any component(*chunk.info,GetOffsetPtr(chunk.blob.data(), stride * index));

			return component;
		}

		s32 create() {

		}

	private:
		struct ChunkData {
			const TypeInfo* info;
			Blob blob;
		};
	private:
		Archetype m_archetype;

		HashMap<Type, ChunkData> m_chunks;
	};

	// 型が見つからない場合の特殊化
	template<typename T, int N>
	struct TypeIndex<T, N> {
		static constexpr int value = -1;
	};

	class ECS {
	public:

		template<class... TComponents>
		Entity create() {
			return create(Archetype::Create<TComponents...>());
		}

		Entity create(StringView archetype) {
			return create(archetype);
		}
		Entity create(Archetype archetype) {
			auto [itr, added] = m_chunks.try_emplace(archetype);
			Chunk& chunk = itr->second;

			Entity entity;
			entity.index = chunk.create();
			entity.archetype = archetype.index();
			entity.version = 0;
		}

		void destroy(Entity entity) {
			//m_chunks[entity.archetype]->destroy(entity.index);
		}

		Any get(Type type, Entity entity) {
			return m_chunks[entity.archetype]->get(type,entity.index);
		}

		template<class TComponent>
		TComponent& get(Entity entity) {
			m_chunks[entity.archetype]->get<TComponent>(entity.index);
		}

		template<class TSystem, class... Args>
		auto update() {
			//TSystem::Update();
		}
		template<class TSystem, class... Args>
		auto update() {
			//TSystem::Update();
		}

		template<class... TComponents>
		void update2(Func<void(TComponents&...)> func) {
			// 全てのComponentのArchetypeの論理積を取る

			Type types[] = { Type::Get<TComponents>()... };

			ArchetypeMask mask;
			mask.flip();

			// 全てのComponentを持つチャンクを抽出
			for (auto& type : types) {
				mask &= m_availables[type]
			}

			for (s32 i = 0; i < mask.size(); ++i) {
				if (mask[i]) {
					m_chunks.at(i);
				}
			}

		}

		void merge(ECS& ecs) {
			// 他のECSのComponentを自分のECSに移す
		}
	private:

		using ArchetypeMask = BitSet<256>;

		HashMap<Type, ArchetypeMask> m_availables;

		HashMap<Archetype,Chunk> m_chunks;
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
	class TransformStartupSystem {
	public:
		static void Update(TransformComponent& transform) {
		}
	};

	void sample() {

		ECS ecs;

		auto entity0 = ecs.create<TransformComponent>();
		auto entity1 = ecs.create<TransformComponent,RigidbodyComponent>();

		auto entity5 = ecs.create("TransformComponent,RigidbodyComponent");
		auto comp = ecs.get(Type("Transform"), entity5);


		ecs.destroy(entity1);
		ecs.get<TransformComponent>(entity0);


		ecs.update<RigidbodySystem>();
		ecs.update<TransformSystem>();


		ECS ecs2;
		// 非同期更新
		{
			ecs2.update<TransformStartupSystem>();
		}

		ecs.merge(ecs2);

	}

}

#endif
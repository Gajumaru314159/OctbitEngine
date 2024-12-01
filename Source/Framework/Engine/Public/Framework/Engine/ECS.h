//***********************************************************
//! @file
//! @brief		シーン
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Reflection/Any.h>
#include <Framework/Core/Reflection/TypeInfo.h>
#include <Framework/Core/String/StringSplitView.h>


#if true

namespace ob::engine2 {

	class Archetype {
	public:
		template<class... TComponents>
		static Archetype Create() {
			Archetype archetype;
			archetype.m_types = { Type::Get<TComponents>()... };
			return archetype;
		}
	public:

		Archetype() = default;

		Archetype(Span<Type> types) {
			for (auto& type : types) {
				m_types.push_back(type);
			}
		}

		Archetype(StringView types) {
			for (auto type : CommaSplitView(types)) {
				if (auto info = TypeInfo::Find(type)) {
					m_types.push_back(info->type);
				}
			}
		}

		auto begin() const {
			return m_types.begin();
		}
		auto end() const {
			return m_types.end();
		}

		bool operator==(const Archetype& other) const {
			return m_types == other.m_types;
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
	};

}

template<>
struct std::hash<ob::engine2::Archetype> {
	size_t operator()(const ob::engine2::Archetype& value)const {
		size_t hash = 0;
		for (auto& type : value) {
			ob::core::Hash::Combine(hash, type);
		}
		return hash;
	}
};



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

	// 型が見つからない場合の特殊化
	template<typename T, int N>
	struct TypeIndex<T, N> {
		static constexpr int value = -1;
	};

	struct Entity {
		u32 index;
		u16 archetype;
		u16 version;
	};



	class AnyVector 
		//: Noncopyable 
	{
	public:
		AnyVector(const TypeInfo& info) 
			: m_info(&info)
		{
			m_constructor = m_info->findConstructor();
			OB_ASSERT(m_constructor, "{}にデフォルトコンストラクタを追加してください", info.type.name());

			const auto l1cache = 16 * 1024;

			m_size = 0;
			m_capacity = l1cache / m_info->stride();
			m_blob.resize(m_capacity * m_info->stride());
		}
		~AnyVector() {
			for (s32 i = 0; i < m_size; ++i) {
				m_info->destroyPlaced(at(i));
			}
		}
		void push_back() {
			auto ptr = at(m_size);
			m_constructor->placedInvoker(ptr, {});
			m_size++;
		}
		size_t size() const {
			return m_size;
		}
		size_t capacity() const {
			return m_capacity;
		}
		bool full() const {
			return m_size == m_capacity;
		}
		void* at(size_t index) {
			return GetOffsetPtr(m_blob.data(), m_info->stride() * index);
		}
		const void* at(size_t index) const {
			return GetOffsetPtr(m_blob.data(), m_info->stride() * index);
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
		ComponentAllocator(const TypeInfo& info)
			: m_info(info)
		{
			m_constructor = m_info.findConstructor();
			OB_ASSERT(m_constructor, "{}にデフォルトコンストラクタを追加してください", info.type.name());
			m_span = m_info.stride();
			m_chunks.emplace_back(m_info);
		}

		s32 push_back() {

			auto* chunk = &m_chunks.back();
			
			// 現在のチャンクがいっぱいの場合新しいチャンクを生成
			if (chunk->full()) {
				chunk = &m_chunks.emplace_back(m_info);
			}
			
			auto index = size();
			
			// チャンクに要素を追加
			chunk->push_back();
			
			return index;
		}

		void init(size_t index) {
			auto ptr = at(index);
			m_info.destroyPlaced(ptr);
			m_constructor->placedInvoker(ptr, {});
		}

		s32 size() const {
			return (m_chunks.size() - 1) * m_span + m_chunks.back().size();
		}

		void* at(size_t index) {
			return m_chunks.at(index / m_span).at(index % m_span);
		}
		const void* at(size_t index) const {
			return m_chunks.at(index / m_span).at(index % m_span);
		}

		template<class T>
		T& at(size_t index) {
			return reinterpret_cast<T*>(at(index));
		}

		template<class T>
		const T& at(size_t index) const {
			return reinterpret_cast<const T*>(at(index));
		}

		const TypeInfo& info() const {
			return m_info;
		}

	private:
		const TypeInfo& m_info;
		Vector<AnyVector> m_chunks;
		size_t m_span;
		const ConstructorInfo* m_constructor;
	};


	//! @brief Archetype毎にComponentを管理するクラス
	class Chunk {
	public:
		Chunk(Archetype archetype)
			: m_archetype(archetype)
		{
			for (auto& type : archetype) {
				if (auto info = TypeInfo::Find(type)) {
					m_chunks.emplace(info->type, *info);
				}
				else {
					throw Exception(Format("{}をリフレクション登録してください", type.name()));
				}
			}
		}

		s32 create() {

			if (m_free.empty()) {
				s32 index = 0;
				for (auto& [type, chunk] : m_chunks) {
					index = chunk.size();
					chunk.push_back();
				}
				return index;
			}

			auto index = m_free.back();
			return index;
		}
		void destroy(s32 index) {
			// TODO 範囲外チェック
			// TODO 二重開放チェック
			m_free.push_back(index);
			for (auto& [type, chunk] : m_chunks) {
				chunk.init(index);
			}
		}

		Any get(Type type, u32 index) {
			auto itr = m_chunks.find(type);
			if (itr == m_chunks.end()) return {};

			auto& chunk = itr->second;
			return Any(chunk.info(), chunk.at(index));
		}

		template<class T>
		T& get(u32 index) {
			return get(Type::Get<T>(), index);
		}

		template<class... TComponents>
		void visit(Func<void(TComponents...)> func) {

			// TODO 型チェック
			auto types = { Type::Get<TComponents>()... };

			ComponentAllocator* chunks[] = { &m_chunks[Type::Get<TComponents>()]... };

			size_t size = 0;
			for (s32 i = 0; i < size; ++i) {
				Tuple<TComponents&...> components{
					(*reinterpret_cast<TComponents*>(m_chunks[Type::Get<TComponents>()].at(i))) ...
				};
				std::apply(func, components);
			}

		}

	private:
		struct ChunkData {
			const TypeInfo* info;
			Blob blob;
		};
	private:
		Archetype m_archetype;

		HashMap<Type, ComponentAllocator> m_chunks;
		Vector<s32> m_free;

	};


	class ECS {
	public:

		//! @brief Archetypeを指定して新しいEntityを生成
		Entity create(Archetype archetype) {
			auto [itr, added] = m_chunks.try_emplace(archetype, archetype);
			if (added) {
				auto index = m_indices.size();
				m_indices[archetype] = index;
				m_archetypes[index] = archetype;
			}

			Chunk& chunk = itr->second;

			Entity entity;
			entity.index = chunk.create();
			entity.archetype = m_indices[archetype];
			entity.version = 0;
			return entity;
		}
		template<class... TComponents>
		Entity create() {
			return create(Archetype::Create<TComponents...>());
		}
		Entity create(StringView archetype) {
			return create(Archetype(archetype));
		}

		//! @brief Archetypeを変更して新しいEntityを生成
		//! @details 元のEntityは破棄される
		Entity map(Archetype archetype, Entity from) {
			return {}; // TODO
		}
		template<class... TComponents>
		Entity map(Entity from) {
			return map(Archetype::Create<TComponents...>(), from);
		}
		Entity map(StringView archetype, Entity from) {
			return map(Archetype(archetype), from);
		}

		//! @brief 同じArchetypeのEntityを複製 
		Entity duplicate(Entity from) {
			return {}; // TODO
		}
		//! @brief Archetypeを指定してEntityを複製
		//! @details 不足するComponentはデフォルトコンストラクタで生成され、余分なComponentは破棄される。
		template<class... TComponents>
		Entity duplicateWith(Entity from) {
			// TODO
			return {};// create(Archetype::Create<TComponents...>());
		}

		//! @brief EntityとComponentを破棄する 
		void destroy(Entity entity) {
			auto itr = m_archetypes.find(entity.archetype);
			if (itr == m_archetypes.end()) return;
			auto& archetype = itr->second;

			auto itr2 = m_chunks.find(archetype);
			if (itr2 == m_chunks.end()) return;
			auto& chunk = itr2->second;

			chunk.destroy(entity.index);
		}

		//! @brief タイプを指定してEntityにバインドされているComponentを取得する
		//! @note  型チェックが発生するため多用は避ける
		Any get(Type type, Entity entity) {
			auto itr = m_archetypes.find(entity.archetype);
			if (itr == m_archetypes.end()) return {};
			auto& archetype = itr->second;

			auto itr2 = m_chunks.find(archetype);
			if (itr2 == m_chunks.end()) return {};
			auto& chunk = itr2->second;

			return chunk.get(type, entity.index);
		}
		Any get(StringView type, Entity entity) {
			return get(Type(type), entity);
		}
		template<class TComponent>
		TComponent& get(Entity entity) {
			return get(Type::Get<TComponent>(), entity).as<TComponent>();
		}




		template<class TSystem, class... Args>
		void update() {
			//TSystem::Update();
		}

		template<class... TComponents>
		void update2(Func<void(TComponents&...)> func) {
			// 全てのComponentのArchetypeの論理積を取る

			Type types[] = { Type::Get<TComponents>()... };

			ArchetypeMask mask;
			mask.flip();

			// 全てのComponentを持つチャンクを抽出
			// NOTE 抽出は毎フレームしなくてよくはないか？
			for (auto& type : types) {
				mask &= m_availables[type]
			}

			// 型チェックはChunk * Archetype.size()回発生する
			for (s32 i = 0; i < mask.size(); ++i) {
				if (mask[i]) {
					m_chunks.at(i);
				}
			}

		}

		void merge(ECS& ecs) {
			// 他のECSのComponentを自分のECSに移す
			// Entityのインデックスを再割り当てしないといけない？
		}
	private:

		using ArchetypeMask = BitSet<256>;

		HashMap<Type, ArchetypeMask> m_availables;

		HashMap<Archetype, Chunk> m_chunks;
		HashMap<Archetype, s32> m_indices;
		HashMap<s32, Archetype> m_archetypes;
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
		static void Update(TransformComponent& transform, RigidbodyComponent& rigidbody) {
			transform.local.position += rigidbody.velocity;
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
		auto entity1 = ecs.create<TransformComponent, RigidbodyComponent>();
		auto entity2 = ecs.create("TransformComponent,RigidbodyComponent");

		ecs.get<TransformComponent>(entity0);
		ecs.get("TransformComponent", entity0).as<TransformComponent>();

		ecs.destroy(entity1);

		// 元EntityからAdd/RemoveしてComponentを再生成
		auto entity3 = ecs.map<TransformComponent>(entity0);

		// 元EntityからAdd/RemoveしてComponentを複製
		auto entity4 = ecs.duplicate(entity0);
		auto entity5 = ecs.duplicateWith<TransformComponent>(entity0);



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
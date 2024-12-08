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

	//! @brief ECSにおけるEntityが持つComponentの型の構成を表すクラス
	class Archetype {
	public:
		//! @brief TypeのリストからArchetypeを生成
		template<class... TComponents>
		static Archetype Create() {
			Archetype archetype;
			archetype.m_types = { Type::Get<TComponents>()... };
			return archetype;
		}
	public:

		//! @brief 空のArchetypeを生成
		Archetype() = default;

		//! @brief Type リストからArchetypeを生成 
		Archetype(Span<Type> types) {
			for (auto& type : types) {
				m_types.push_back(type);
			}
		}

		//! @brief カンマ区切りの文字列からArchetypeを生成
		Archetype(StringView types) {
			for (auto type : CommaSplitView(types)) {
				if (auto info = TypeInfo::Find(type)) {
					m_types.push_back(info->type);
				} else {
					throw Exception(Format("{}をリフレクション登録してください", type));
				}
			}
		}

		//! @brief Typeの開始イテレータ 
		auto begin() const {
			return m_types.begin();
		}

		//! @brief Typeの終了イテレータ 
		auto end() const {
			return m_types.end();
		}

		//! @brief 等価判定
		bool operator==(const Archetype& other) const {
			return m_types == other.m_types;
		}

		//! @brief 否等価判定
		bool operator!=(const Archetype& other) const {
			return m_types != other.m_types;
		}

	private:
		FixedVector<Type, 8> m_types;
	};

}

//! @cond
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
//! @endcond

namespace ob::engine2 {

	//! @brief Entity
	struct Entity {
		u32 index;
		u16 archetype;
		u16 version;
	};

	struct Component { OB_RTTI(); };

	//! @brief TypeInfoを渡すことでpush_backできるVector
	class AnyVector 
		: Noncopyable 
	{
	public:
		AnyVector(const TypeInfo& info) 
			: m_info(&info)
		{
			OB_ASSERT(info.isSuperClassOf<Component>(), "{}はComponentを継承していません", info.type.name());
			auto ctor = m_info->findConstructor();
			OB_ASSERT(ctor, "{}にデフォルトコンストラクタを追加してください", info.type.name());
			m_constructor = ctor->placedInvoker;

			const auto l1cache = 16 * 1024;

			m_size = 0;
			m_capacity = l1cache / m_info->stride();
			m_blob.resize(m_capacity * m_info->stride());

			m_components = reinterpret_cast<Component*>(m_blob.data());
			m_stride = m_info->stride();
		}
		~AnyVector() {
			for (s32 i = 0; i < m_size; ++i) {
				m_info->destroyPlaced(at(i));
			}
		}
		void push_back() {
			OB_ASSERT(full()==false, "空きのないAnyVectorに要素を追加しました");
			auto ptr = at(m_size);
			m_constructor(ptr, {});
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
		PlacedConstructorInvoker m_constructor;
		Blob m_blob;
		size_t m_size;
		size_t m_capacity;

		Component* m_components;
		size_t m_stride;
	};

	//! @brief Componentをキャッシュラインに収まるようにメモリ上連続するようにアロケートするコンテナ
	class ComponentContainer {
	public:
		ComponentContainer(const TypeInfo& info)
			: m_info(info)
		{
			auto ctor = m_info.findConstructor();
			OB_ASSERT(ctor, "{}にデフォルトコンストラクタを追加してください", info.type.name());
			m_constructor = ctor->placedInvoker;
			m_destructor = m_info.placedDestructor;
			m_blocks.emplace_back(std::make_unique<AnyVector>(m_info));
			m_span = m_blocks.back()->capacity();
			m_usage = 0;
			m_stride = m_info.stride();
		}

		s32 push_back() {

			auto* chunk = &m_blocks.back();
			
			// 現在のチャンクがいっぱいの場合新しいチャンクを生成
			if ((*chunk)->full()) {
				chunk = &m_blocks.emplace_back(std::make_unique<AnyVector>(m_info));
			}
			
			auto index = size();
			
			// チャンクに要素を追加
			(*chunk)->push_back();

			m_usage++;
			
			return index;
		}

		void init(size_t index) {
			auto ptr = at(index);
			m_destructor(ptr);
			m_constructor(ptr, {});
		}

		s32 size() const {
			return (m_blocks.size() - 1) * m_span + m_blocks.back()->size();
		}

		void* at(size_t index) {
			return m_blocks.at(index / m_span)->at(index % m_span);
		}
		const void* at(size_t index) const {
			return m_blocks.at(index / m_span)->at(index % m_span);
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
		Vector<UPtr<AnyVector>> m_blocks;
		size_t m_usage;
		size_t m_stride;
		size_t m_span;
		PlacedConstructorInvoker m_constructor;
		PlacedDestructorInvoker m_destructor;
	};


	//! @brief Archetype毎にComponentを管理するクラス
	class Chunk {
	public:
		Chunk(Archetype archetype)
			: m_archetype(archetype)
		{
			for (auto& type : archetype) {
				if (auto info = TypeInfo::Find(type)) {
					m_containers.emplace(info->type, *info);
				}
				else {
					throw Exception(Format("{}をリフレクション登録してください", type.name()));
				}
			}
		}

		s32 create() {

			if (m_free.empty()) {
				s32 index = 0;
				for (auto& [type, chunk] : m_containers) {
					index = chunk.size();
					chunk.push_back();
				}
				m_used.push_back(true);
				return index;
			}

			auto index = m_free.back();
			m_used[index] = true;

			return index;
		}
		void destroy(s32 index) {
			// TODO 範囲外チェック
			// TODO 二重開放チェック
			m_free.push_back(index);
			m_used[index] = false;
			for (auto& [type, chunk] : m_containers) {
				chunk.init(index);
			}
		}

		Any get(Type type, u32 index) {
			auto itr = m_containers.find(type);
			if (itr == m_containers.end()) return {};

			auto& chunk = itr->second;
			return Any(chunk.info(), chunk.at(index));
		}

		template<class T>
		T& get(u32 index) {
			return get(Type::Get<T>(), index);
		}

		template<class... TComponents>
		void visit(Func<void(TComponents&...)>& func) {
			visit_impl(func, std::make_index_sequence<sizeof...(TComponents)>());
		}

	private:

		template<class... TComponents, size_t ...I>
		void visit_impl(Func<void(TComponents&...)>& func, std::index_sequence<I...>) {

			ComponentContainer* chunks[] = { (&m_containers.find(Type::Get<TComponents>())->second) ... };

			for (s32 i = 0; i < m_used.size(); ++i) {
				func((*reinterpret_cast<TComponents*>(chunks[I]->at(i))) ...);
			}

		}

	private:
		Archetype m_archetype;
		HashMap<Type, ComponentContainer> m_containers;
		Vector<bool> m_used;
		Vector<s32> m_free;
	};


	class ECS {
	public:
		template<class... TCompoennts>
		struct is_all_component : std::conjunction<std::is_base_of<Component, TCompoennts>...> {};
	public:

		//! @brief Archetypeを指定して新しいEntityを生成
		Entity create(Archetype archetype) {
			auto [itr, added] = m_chunks.try_emplace(archetype, archetype);
			if (added) {
				auto index = m_indices.size();
				m_indices[archetype] = index;
				m_archetypes[index] = archetype;

				for (auto& type : archetype) {
					m_availables[type].set(index, true);
				}
			}

			Chunk& chunk = itr->second;

			Entity entity;
			entity.index = chunk.create();
			entity.archetype = m_indices[archetype];
			entity.version = 0;
			return entity;
		}
		template<class... TComponents>
		auto create() -> std::enable_if_t<is_all_component<TComponents...>::value,Entity> {
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
		auto map(Entity from) -> std::enable_if_t<is_all_component<TComponents...>::value, Entity> {
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
		auto duplicateWith(Entity from) -> std::enable_if_t<is_all_component<TComponents...>::value, Entity> {
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


		template<class... TComponents>
		void update(Func<void(TComponents&...)>& func) {
			if (!func)return;

			// 全てのComponentのArchetypeの論理積を取る

			Type types[] = { Type::Get<TComponents>()... };
			
			ArchetypeMask mask;
			mask.flip();
			
			// 全てのComponentを持つチャンクを抽出
			// NOTE 抽出は毎フレームしなくてよくはないか？
			for (auto& type : types) {
				mask &= m_availables[type];
			}
			
			// 型チェックはChunk * Archetype.size()回発生する
			for (s32 i = 0; i < m_indices.size(); ++i) {
				if (mask.test(i)) {
					
					auto& archetype = m_archetypes[i];
					
					auto itr = m_chunks.find(archetype);
					if (itr == m_chunks.end()) continue;
					auto& chunk = itr->second;

					chunk.visit<TComponents...>(func);
				}
			}

		}

		template<class... TComponents,class... Args>
		void update(void(*func)(TComponents&...),Args&&... args) {
			Func<void(TComponents&...)> f(func);
			update(f);
		}

		template<class TSystem,class... Args>
		void update(Args&&... args) {
			update(TSystem::Update,args...);
		}

		void merge(ECS& ecs) {
			// 他のECSのComponentを自分のECSに移す
			// Entityのインデックスを再割り当てしないといけない？
		}
	private:

		using ArchetypeMask = BitSet<1024>;

		HashMap<Archetype, Chunk> m_chunks;
		HashMap<Archetype, s32> m_indices;
		HashMap<s32, Archetype> m_archetypes;
		HashMap<Type, ArchetypeMask> m_availables;
	};



}

#endif
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>
#include <Framework/Core/Template/Container/Vector.h>
#include <Framework/Core/Template/Container/Set.h>
#include <Framework/Core/Template/Utility/Function.h>

namespace ob::core {

	//! @brief  
	template<class T, T* T::* pMember>
	class IntrusiveQueue {
	public:
		using this_type = IntrusiveQueue<T, pMember>;
	public:
		IntrusiveQueue() = default;
		IntrusiveQueue(this_type&& rhs)noexcept {
			m_top = rhs.m_top;
			m_last = rhs.m_last;
			rhs.clear();
		}
		~IntrusiveQueue() {
			clear();
		}
		this_type& operator=(this_type&& rhs)noexcept {
			return *this = rhs;
		}

		IntrusiveQueue(const this_type&) = delete;
		IntrusiveQueue& operator=(const this_type&) = delete;

		void clear() {
			while (dequeue()) {
				// NTOTHING TO DO
			}
		}
		void enqueue(T& instance) {
			set_next(&instance, nullptr);
			if (m_top == nullptr) {
				m_top = &instance;
			}
			if (m_last != nullptr) {
				set_next(m_last, &instance);
			}
			m_last = &instance;
		}
		T* dequeue() {
			auto result = m_top;
			if (result) {
				auto next = get_next(result);
				if (next == nullptr) {
					m_last = nullptr;
				}
				m_top = next;
				set_next(result, nullptr);
			}
			return result;
		}
		T* peek() {
			return m_top;
		}

		bool empty()const {
			return m_top == nullptr;
		}
	private:
		T*& get_next(T* node) {
			return (T*&)(node->*pMember);
		}
		void set_next(T* node, T* next) {
			(node)->*pMember = next;
		}
	private:
		T* m_top = nullptr;
		T* m_last = nullptr;
	};


	//! @brief		ハンドル・マネージャ
	//! @dettails	このクラスは、Handleを使用してインスタンスを管理するためのハンドルマネージャです。
	template<class T,class Deleter = std::default_delete<T>>
	class HandleManager {
	public:

		class Handle {
			friend class HandleManager;
		public:
			Handle() = default;
			bool operator==(const Handle& rhs)const { return m_id == rhs.m_id; }
			bool operator!=(const Handle& rhs)const { return m_id != rhs.m_id; }
			bool operator<(const Handle& rhs)const { return  m_id < rhs.m_id || ((rhs.m_id <= m_id) && m_owner < rhs.m_owner); }
			bool empty()const { return m_owner == nullptr; }
			void clear() { m_id = 0; m_owner = nullptr; }
			auto id()const { return m_id; }
			T* value()const { return m_owner ? m_owner->at(*this) : nullptr; }
		private:
			union {
				u32 m_id;
				struct {
					u32 m_index : 24;
					u32 m_version : 8;
				};
			};
			HandleManager* m_owner = nullptr;
		};

		struct Node {
			Node* next = nullptr;
			T* instance = nullptr;
			u32 version = 0;
		};

	public:

		HandleManager(size_t capacity) {
			m_nodes.resize(capacity);
			for (auto& node : m_nodes) {
				m_queue.enqueue(node);
			}
		}

		~HandleManager() {
			OB_ASSERT(std::all_of(m_nodes.begin(), m_nodes.end(), [](const Node& node) {return node.instance == nullptr; }),"未開放のインスタンスが含まれています。");
		}

		Handle add(T* instance) {
			auto node = m_queue.dequeue();
			OB_ASSERT(node != nullptr, "Out of capacity");

			node->instance = instance;

			Handle handle;
			handle.m_owner = this;
			handle.m_index = node - m_nodes.data();
			handle.m_version = node->version;

			return handle;
		}

		void remove(const Handle& handle) {
			if (handle.empty())return;
			if (handle.m_owner != this) return;

			auto& node = m_nodes.at(handle.m_index);

			if (node.version != handle.m_version)return;

			Deleter{}(node.instance);

			node.instance = nullptr;
			node.version++;

			m_queue.enqueue(node);
		}

		T* at(const Handle& handle)const {
			if (handle.empty())return nullptr;
			if (handle.m_owner != this) return nullptr;

			auto& node = m_nodes.at(handle.m_index);

			if (node.version != handle.m_version)return nullptr;

			return node.instance;
		}

	private:
		Vector<Node> m_nodes;
		IntrusiveQueue<Node, &Node::next> m_queue;
	};

	template<class T, class Deleter = std::default_delete<T>>
	class SyncHandleManager {
	public:
		using manager_type = HandleManager<T, Deleter>;
		using handle_type = typename manager_type::Handle;
	public:
		struct Hasher {
			size_t operator()(const handle_type& handle)const {
				return handle.id();
			}
		};
	private:
		SpinLock m_lock;
		HandleManager<T> m_manager;
		Vector<handle_type> m_disposeQueue;
		HashSet<handle_type, Hasher> m_actives;
		HashSet<u32> m_disposedSet;
	public:

		SyncHandleManager(size_t capacity) 
			: m_manager(capacity)
		{

		}

		~SyncHandleManager() {
			for (auto& handle : m_actives) {
				remove(handle);
			}
			update();
		}

		handle_type add(T* instance) {
			ScopeLock lock(m_lock);
			auto handle = m_manager.add(instance);
			m_actives.emplace(handle);
			return handle;
		}

		void remove(const handle_type& handle) {
			if (handle.empty())return;
			ScopeLock lock(m_lock);
			if (m_disposedSet.count(handle.id()))return;
			m_disposedSet.emplace(handle.id());
			m_disposeQueue.push_back(handle);
		}
		T* at(const handle_type& handle)const {
			return m_manager.at(handle);
		}

		void visit(Func<void(T&)>visitor) {
			ScopeLock lock(m_lock);
			for (auto& handle : m_actives) {
				if (auto instance = handle.value()) {
					visitor(*instance);
				}
			}
		}

		void update() {
			ScopeLock lock(m_lock);
			for (auto& handle : m_disposeQueue) {
				m_actives.erase(handle);
				m_manager.remove(handle);
			}
			m_disposeQueue.clear();
			m_disposedSet.clear();
		}
	};
}
//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#if 0
#include <Framework/Core/Utility/Algorithm.h>
#include <Framework/Graphics/Render/RenderPassBuilder.h>

namespace ob::graphics {

	void RenderPassBuilder::connect(const RenderPassConnection& connection) {
		m_connections.push_back(connection);
	}

	void RenderPassBuilder::connect(const String& from, const String& to) {
		connect({from,to});
	}

	void RenderPassBuilder::flush() {

		struct A {
			s32 index;
			Vector<s32> connections;

			A() = default;
			A(size_t index) : index(index) {}
		};

		Map<String, A> requiredPasses;
		Map<s32, String> requiredPassesUnmap;
		// Map<String, RenderPassDesc> passes;

		auto stlip_pass = [](const String& str) -> String {
			auto index = str.find('.');
			if (index == String::npos) return "";
			return String(str.substr(0,index));
		};

		for (auto& connection : m_connections) {

			// 接続先のパスを登録
			requiredPasses.try_emplace(stlip_pass(connection.from), requiredPasses.size());
			requiredPasses.try_emplace(stlip_pass(connection.to), requiredPasses.size());

			// 逆変換を登録
			requiredPassesUnmap[requiredPasses[stlip_pass(connection.to)].index] = stlip_pass(connection.to);
			requiredPassesUnmap[requiredPasses[stlip_pass(connection.from)].index] = stlip_pass(connection.from);

			// 接続情報を登録
			s32 to = requiredPasses[stlip_pass(connection.to)].index;
			requiredPasses[stlip_pass(connection.from)].connections.push_back(to);

			m_connections2.connect(connection.from, connection.to);
		}

		// TODO Globalをどこに持たせるか


		// 必要なパスが登録済みかチェック
		for (auto& [name, requiredPass] : requiredPasses) {
			if (!m_localExecutors.contains(name)) {
				LOG_ERROR("不明なパス {}", name);
				requiredPasses.clear();
				break;
			}
		}

		// パスの依存関係を構築
		Vector<Vector<s32>> graph(requiredPasses.size());
		for (auto& [name, node] : requiredPasses) {
			graph[node.index] = node.connections;
		}

		// TODO RenderFeatureの依存関係解決

		// トポロジーソート
		Vector<s32> sorted = TopologicalSort(graph);

		// トポロジーソートの結果を元にパスを作成
		for (auto& index : sorted) {
			m_localPasses.push_back(requiredPassesUnmap[index]);
		}

	}

	void RenderPassBuilder::render(FG& fg) {
		for (auto type : m_localPasses) {
			m_globalExecutors.at(type)(fg);
		}
	}
	void RenderPassBuilder::render(FG& fg, RenderView& view) {
		for (auto type : m_localPasses) {
			m_localExecutors.at(type)(fg, view);
		}
	}

}
#endif
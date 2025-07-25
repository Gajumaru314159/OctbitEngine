//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Utility/Algorithm.h>

namespace ob::core {

	//! @brief 有向グラフのトポロジカルソートを行い、頂点の順序を返します。
	//! @param graph 隣接リスト形式で表現された有向グラフ。各頂点ごとに、隣接する頂点のリストを持つ2次元ベクターです。
	//! @return トポロジカルソートされた頂点の順序を表す整数型ベクター。グラフに閉路が存在する場合は空のベクターを返します。
	Vector<s32> TopologicalSort(const Vector<Vector<s32>>& graph) {

		Vector<s32> indegrees(graph.size());

		// 入次数を計算
		for (const auto& v : graph) {
			for (const auto& to : v) {
				++indegrees[to];
			}
		}

		// 入次数が0の頂点をすべてキューに追加
		PriorityQueue<s32, Vector<s32>, std::greater<>> queue;
		for (s32 i = 0; i < graph.size(); ++i) {
			if (indegrees[i] == 0) {
				queue.push(i);
			}
		}

		// ソート
		Vector<s32> result;
		while (!queue.empty()) {

			const int from = queue.top(); queue.pop();
			result.push_back(from);

			for (const auto& to : graph[from]) {
				if (--indegrees[to] == 0) {
					queue.push(to);
				}
			}

		}

		// 閉路が存在する場合はから配列を返す
		if (result.size() < graph.size()) {
			return{};
		}

		return result;
	}

}
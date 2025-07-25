//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/CorePrivate.h>

namespace ob::core {

	//! @brief 有向グラフのトポロジカルソートを行い、頂点の順序を返します。
	//! @param graph 隣接リスト形式で表現された有向グラフ。各頂点ごとに、隣接する頂点のリストを持つ2次元ベクターです。
	//! @return トポロジカルソートされた頂点の順序を表す整数型ベクター。グラフに閉路が存在する場合は空のベクターを返します。
	Vector<s32> TopologicalSort(const Vector<Vector<s32>>& graph);

}
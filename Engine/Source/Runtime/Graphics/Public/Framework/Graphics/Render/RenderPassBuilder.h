//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#if 0
#pragma once
#include <Framework/Graphics/Render/RenderSceneDesc.h>
#include <Framework/Core/Reflection/AnyContainer.h>

#include <Framework/Graphics/FrameGraph/FG.h>


namespace ob::graphics {

	class RenderPassBuilder {
	public:

		template<class T>
		void add(String name = "") {
			if (name.empty()) name = String(Type::Get<T>().shortName());
			
			// Passの実体を生成
			m_passes.get<T>();

			// Global/Localごとにパス実行関数を登録
			//constexpr if (T::is_global) {
			//	m_globalExecutor[name] = [this](FG& fg, FGBlackboard& blackboard) {
			//		m_passes.get<T>().render(fg, T::Input(blackboard)).output(blackboard);
			//	};
			//} else
			{
				m_localExecutors[name] = [this,name2=name](FG& fg, RenderView& view) {

					typename T::Input input;
					input.connect(m_connections2);

					typename T::Output output = m_passes.get<T>().render(fg, view, input);
					
					output.connect(m_connections2);
				};
			}

		}

		void connect(const RenderPassConnection& connection);
		void connect(const String& from, const String& to);
		void flush();
		void render(FG& fg);
		void render(FG& fg, RenderView& view);

	private:
		// 接続情報
		Vector<RenderPassConnection> m_connections;

		FGConnections m_connections2;

		// 実体
		AnyContainer m_passes;

		// 評価順
		Vector<String> m_globalPasses;
		Vector<String> m_localPasses;

		// 実行関数
		HashMap<String, Func<void(FG&)>> m_globalExecutors;
		HashMap<String, Func<void(FG&, RenderView&)>> m_localExecutors;
	};

}
#endif
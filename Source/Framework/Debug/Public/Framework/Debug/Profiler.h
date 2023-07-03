//***********************************************************
//! @file
//! @brief		プロファイラ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Core.h>
#include <Framework/Core/Log/Logger.h>
#include <Plugins/ImGui/ImGui.h>

namespace ob::debug {


	class Profiler {
	public:
		Profiler();
		void update();

	private:

		struct TimeBuffer {
		public:
			s32 size;
			s32 offset;
			Array<Vec2> data;
		public:
			TimeBuffer(s32 size = 2000) {
				size = size;
				offset = 0;
				data.reserve(size);
			}
			void add(f32 x, f32 y) {
				if (data.size() < size)
					data.push_back(Vec2(x, y));
				else {
					data[offset] = Vec2(x, y);
					offset = (offset + 1) % size;
				}
			}
			void reset() {
				data.clear();
				offset = 0;
			}
		};

	private:
		TimeBuffer m_timeBuffer;
		f32 m_time;
		f32 m_history;
	};

}
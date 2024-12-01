//***********************************************************
//! @file
//! @brief		Box のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/TypeBuilder.h>
#include <Framework/Core/Reflection/TypeRegister.h>
#include <Framework/Core/Reflection/TypeInfoPrinter.h>
#include <nlohmann/json.hpp>

using namespace ob;

/*
template<class... TComponent>
class Chunk {
public:

	void resize(size_t size) {
		std::get<0>(m_containers).resize(size);
		std::get<1>(m_containers).resize(size);
		std::get<2>(m_containers).resize(size);
	}

	template<class T>
	T& get(s32 index) {
		return std::get<Vector<T>>(m_containers).at(index);
	}

	template<class... Args>
	void update(Func<void(Args&...)> func) {
		auto len = std::get<0>(m_containers).size();
		for (s32 i = 0; i < len; ++i) {
			// 関数呼び出し
			func(get<Args>(i)...);
		}
	}
	template<class L>
	void update(L func) {
		Func f = func;
		
	}
private:
	Tuple<Vector<TComponent>...> m_containers;
};

namespace {
	struct A {
		int value = 0;
	};
	struct B {
		int value = 1;
	};
	struct C {
		int value = 2;
	};
}

TEST(ECS, Construct) {

	Chunk<A, B, C> chunk;

	chunk.resize(100);


	chunk.update([](A& a, C& c) {
		a.value = 10;
		c.value = 30;
	});

	CallBreakPoint();

}
*/
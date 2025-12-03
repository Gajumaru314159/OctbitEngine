//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Template/Container/BitSet.h>
#include <Framework/Core/Template/Utility/MiniDelegate.h>

#if 0
using namespace ob;

BitSet<4> s_result;

struct A {
public:

	A() {

		MiniDelegate<void()> d00(*this, &A::func00);
		MiniDelegate<bool()> d01(*this, &A::func01);
		MiniDelegate<void(bool)> d02(*this, &A::func02);
		MiniDelegate<bool(bool)> d03(*this, &A::func03);

		d00();
		d01();
		d02(true);
		d03(true);
		
		MiniDelegate<void()> d10(*this, &A::func10);
		MiniDelegate<bool()> d11(*this, &A::func11);
		MiniDelegate<void(bool)> d12(*this, &A::func12);
		MiniDelegate<bool(bool)> d13(*this, &A::func13);

		d10();
		d11();
		d12(true);
		d13(true);
		
		MiniDelegate<void()> d20(&A::func20);
		MiniDelegate<bool()> d21(&A::func21);
		MiniDelegate<void(bool)> d22(&A::func22);
		MiniDelegate<bool(bool)> d23(&A::func23);

		d20();
		d21();
		d22(true);
		d23(true);


		MiniDelegate<void()> d04 = { *this, &A::func00 };
		MiniDelegate<void()> d14 = { *this, &A::func10 };
		MiniDelegate<void()> d24 = {&A::func20};

	}

	void constTest() {		
		MiniDelegate<void()> d10(*this, &A::func00);
		MiniDelegate<bool()> d11(*this, &A::func01);
		MiniDelegate<void(bool)> d12(*this, &A::func02);
		MiniDelegate<bool(bool)> d13(*this, &A::func03);
	}

	void func00() {
		m_result[0] = true;
	}

	bool func01() {
		m_result[1] = true;
		return true;
	}

	void func02(bool a) {
		m_result[2] = true;
	}

	bool func03(bool a) {
		m_result[3] = true;
		return true;
	}

	void func10() const{
		m_result[4] = true;
	}

	bool func11() const {
		m_result[5] = true;
		return true;
	}

	void func12(bool a) const {
		m_result[6] = true;
	}

	bool func13(bool a) const {
		m_result[7] = true;
		return true;
	}

	static void func20() {
		s_result[0] = true;
	}

	static bool func21() {
		s_result[1] = true;
		return true;
	}

	static void func22(bool a) {
		s_result[2] = true;
	}

	static bool func23(bool a) {
		s_result[3] = true;
		return true;
	}
public:
	bool success() {
		return m_result.all() && s_result.all();
	}
private:
	mutable BitSet<8> m_result;
};

TEST(MiniDelegate, Test) {

	A a;
	EXPECT_TRUE(a.success());

}
#endif
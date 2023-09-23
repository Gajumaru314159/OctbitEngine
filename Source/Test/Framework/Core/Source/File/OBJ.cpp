//***********************************************************
//! @file
//! @brief		Path のテスト
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/File/OBJ.h>
#include <filesystem>

using namespace ob;

TEST(OBJ, Test)
{
	MTL mtl;
	mtl.newmtl("test0");
	mtl.diffuse(Color::Red);
	mtl.newmtl("test1");
	mtl.diffuse(Color::Yellow);
	mtl.save("test.mtl");

	OBJ obj;
	obj.comment(u8"テスト出力");
	obj.mtl("test.mtl");
	obj.material("test0");
	obj.poly_pn(
		Vec3(0, 0, 0), Vec3(0, 1, -1),
		Vec3(1, 0, 0), Vec3(0, 1, -1),
		Vec3(1, 1, 0), Vec3(0, -1, -1),
		Vec3(0, 1, 0), Vec3(0, -1, -1)
	);

	obj.material("test1");
	obj.poly_pn(
		Vec3(0, 0, 0), Vec3(0, 1, 1),
		Vec3(1, 0, 0), Vec3(0, 1, 1),
		Vec3(1, 0, 1), Vec3(0, 1, -1),
		Vec3(0, 0, 1), Vec3(0, 1, -1)
	);

	obj.save("test.obj");
}




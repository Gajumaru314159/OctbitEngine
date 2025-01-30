//***********************************************************
//! @file
//! @brief		コンポーネント
//! @author		Gajumaru
//***********************************************************
#include <Framework/Engine/Component/TransformComponent.h>
#include <fstream>

#define DECLSPEC extern "C" __declspec(dllexport)

using namespace ob::core;

DECLSPEC void __stdcall Camera_SetPosition(ob::engine::TransformComponent* transform, f32 x, f32 y, f32 z) {
	transform->setLocalPosition({ x,y,z });
}


DECLSPEC void __stdcall ExportText() {
	std::ofstream s("D:/sample.txt");
	s << "Hello World!";
}
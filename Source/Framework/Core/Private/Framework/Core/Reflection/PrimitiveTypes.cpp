//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/TypeBuilder.h>

OB_DEFINE_PRIMITIVE_INFO(ob::core::s64) {
	desc("64bit符号付き整数型");
	tag("DisplayName", "s64");
	tag("C#", "System.Int64");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::u64) {
	desc("64bit符号なし整数型");
	tag("DisplayName", "u64");
	tag("C#", "System.UInt64");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::s32) {
	desc("32bit符号付き整数型");
	tag("DisplayName", "s32");
	tag("C#", "System.Int32");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::u32) {
	desc("32bit符号なし整数型");
	tag("DisplayName", "u32");
	tag("C#", "System.UInt32");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::s16) {
	desc("16bit符号付き整数型");
	tag("DisplayName", "s16");
	tag("C#", "System.Int16");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::u16) {
	desc("16bit符号なし整数型");
	tag("DisplayName", "u16");
	tag("C#", "System.UInt16");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::s8) {
	desc("8bit符号付き整数型");
	tag("DisplayName", "s8");
	tag("C#", "System.SByte");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::u8) {
	desc("8bit符号なし整数型");
	tag("DisplayName", "u8");
	tag("C#", "System.Byte");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::f64) {
	tag("DisplayName", "f64");
	tag("C#", "System.Double");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::f32) {
	tag("DisplayName", "f32");
	tag("C#", "System.Single");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::Char) {
	desc("文字型");
	tag("C#", "System.Char");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::Bool) {
	desc("真偽値型");
	tag("C#", "System.Boolean");
}

OB_DEFINE_CLASS_INFO(ob::core::String) {
	desc("文字列型");
	tag("DisplayName", "ob::core::String");
	tag("C#", "System.String");
	constructor<const Char*>().desc("コンストラクタ");
	constructor<String>().desc("コンストラクタ");
}



OB_DEFINE_CLASS_INFO(ob::core::Vec2) {
	desc("2次元ベクトル");
	tag("C#", "Common.Math.Vector2");
	constructor().desc("デフォルトコンストラクタ");
	constructor<f32>().desc("コンストラクタ(すべての要素を同じ値で初期化)");
	constructor<f32, f32>().desc("コンストラクタ(要素を指定して初期化)");
}

OB_DEFINE_CLASS_INFO(ob::core::Vec3) {
	desc("3次元ベクトル");
	tag("C#", "Common.Math.Vector3");
	constructor().desc("デフォルトコンストラクタ");
	constructor<f32>().desc("コンストラクタ(すべての要素を同じ値で初期化)");
	constructor<f32, f32, f32>().desc("コンストラクタ(要素を指定して初期化)");
}

OB_DEFINE_CLASS_INFO(ob::core::Vec4) {
	desc("4次元ベクトル");
	tag("C#", "Common.Math.Vector4");
	constructor().desc("デフォルトコンストラクタ");
	constructor<f32>().desc("コンストラクタ(すべての要素を同じ値で初期化)");
	constructor<f32, f32, f32, f32>().desc("コンストラクタ(要素を指定して初期化)");
}

OB_DEFINE_CLASS_INFO(ob::core::IntVec2) {
	desc("2次元整数ベクトル");
	tag("C#", "Common.Math.IntVec2");
	constructor().desc("デフォルトコンストラクタ");
	constructor<s32>().desc("コンストラクタ(すべての要素を同じ値で初期化)");
	constructor<s32, s32>().desc("コンストラクタ(要素を指定して初期化)");
}

OB_DEFINE_CLASS_INFO(ob::core::IntVec3) {
	desc("3次元整数ベクトル");
	tag("C#", "Common.Math.IntVec3");
	constructor().desc("デフォルトコンストラクタ");
	constructor<s32>().desc("コンストラクタ(すべての要素を同じ値で初期化)");
	constructor<s32, s32, s32>().desc("コンストラクタ(要素を指定して初期化)");
}

OB_DEFINE_CLASS_INFO(ob::core::IntVec4) {
	desc("4次元整数ベクトル");
	tag("C#", "Common.Math.IntVec4");
	constructor().desc("デフォルトコンストラクタ");
	constructor<s32>().desc("コンストラクタ(すべての要素を同じ値で初期化)");
	constructor<s32, s32, s32, s32>().desc("コンストラクタ(要素を指定して初期化)");
}

OB_DEFINE_CLASS_INFO(ob::core::Rot) {
	desc("回転");
	tag("C#", "Common.Math.Rotation");
	constructor().desc("デフォルトコンストラクタ");
	constructor<f32, f32, f32>().desc("コンストラクタ(要素を指定して初期化)");
}

OB_DEFINE_CLASS_INFO(ob::core::Quat) {
	desc("回転");
	tag("C#", "Common.Math.Quaternion");
	constructor().desc("デフォルトコンストラクタ");
	constructor<f32, f32, f32, f32>().desc("コンストラクタ(要素を指定して初期化)");
}

OB_DEFINE_CLASS_INFO(ob::core::Color) {
	desc("色");
	tag("C#", "Common.Graphics.Color");
	constructor().desc("デフォルトコンストラクタ");
	constructor<f32, f32, f32, f32>().desc("コンストラクタ(各要素を指定して初期化)");
	constructor<f32, f32>().desc("コンストラクタ(輝度とアルファ値を指定して初期化)");
}

OB_DEFINE_CLASS_INFO(ob::core::IntColor) {
	desc("色");
	//tag("C#", "Common.Graphics.Color");
	constructor().desc("デフォルトコンストラクタ");
	constructor<s32, s32, s32, s32>().desc("コンストラクタ(各要素を指定して初期化)");
	constructor<s32, s32>().desc("コンストラクタ(輝度とアルファ値を指定して初期化)");
}

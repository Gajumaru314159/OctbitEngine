//***********************************************************
//! @file
//! @brief		型情報
//! @author		Gajumaru
//***********************************************************
#include <Framework/Core/Reflection/TypeBuilder.h>

namespace  ob::core::internal{

	//@―---------------------------------------------------------------------------
	//! @brief		Primitive型情報ビルダー
	//@―---------------------------------------------------------------------------
	class PrimitiveBuilder : public TagBuilder {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief		コンストラクタ
		//@―---------------------------------------------------------------------------
		PrimitiveBuilder(TypeInfo& info)
			: TagBuilder(info)
			, m_info(info)
		{
		}

	protected:
		TypeInfo& m_info;
	};

	//@―---------------------------------------------------------------------------
	//! @brief		Primitive型情報ビルダー
	//@―---------------------------------------------------------------------------
	template<class _T>
	class PrimitiveBuilderTemplate :public PrimitiveBuilder {
	public:
		using T = _T;
	public:

		//@―---------------------------------------------------------------------------
		//! @brief			コンストラクタ
		//@―---------------------------------------------------------------------------
		PrimitiveBuilderTemplate()
			: PrimitiveBuilder(TypeInfoManager::Instance().registerInfo(Type::Get<T>()))
		{
			// コンストラクタ登録(デフォルト)
			{
				auto& ctor = m_info.constructors.emplace_back();
				ctor.invoker = [](Span<Any> args) { return Any::Create<T>(); };
			}

			// コンストラクタ登録(初期値あり)
			{
				auto& ctor = m_info.constructors.emplace_back();
				ctor.arguments = { {Type::Get<T>(),"value"} };
				ctor.invoker = [](Span<Any> args) { return Any::Create<T>(args[0].as<T>()); };
			}

			// デストラクタ登録
			{
				m_info.destructor = [](void* ptr) { OB_ASSERT(ptr, "ptrがnullです"); delete reinterpret_cast<T*>(ptr); };
				m_info.destructor = [](void* ptr) { OB_ASSERT(ptr, "ptrがnullです"); reinterpret_cast<T*>(ptr)->~T(); };
			}

			// コピー
			if constexpr (std::is_copy_assignable<T>::value) {
				m_info.copyInvoker = [](const void* ptr) { return (void*)new T(*reinterpret_cast<const T*>(ptr)); };
			}

			// タイプ登録
			Register();
		}

		//@―---------------------------------------------------------------------------
		//! @brief			タイプ登録
		//@―---------------------------------------------------------------------------
		void Register();

	};

}


//@―---------------------------------------------------------------------------
//! @brief		Class型情報の定義
//! @details	
//! @note		
//@―---------------------------------------------------------------------------
#define OB_DEFINE_PRIMITIVE_INFO(type)\
namespace type_info_builder::type {\
	void Register() {\
		::ob::core::internal::PrimitiveBuilderTemplate<::type> builder{};\
	}\
}\
template<> void ::ob::core::internal::PrimitiveBuilderTemplate<::type>::Register()



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
	//tag("C#", "Common.Math.Vector2");
	constructor().desc("デフォルトコンストラクタ");
	constructor<s32>().desc("コンストラクタ(すべての要素を同じ値で初期化)");
	constructor<s32, s32>().desc("コンストラクタ(要素を指定して初期化)");
}

OB_DEFINE_CLASS_INFO(ob::core::IntVec3) {
	desc("3次元整数ベクトル");
	tag("C#", "Common.Math.Vector3");
	constructor().desc("デフォルトコンストラクタ");
	constructor<s32>().desc("コンストラクタ(すべての要素を同じ値で初期化)");
	constructor<s32, s32, s32>().desc("コンストラクタ(要素を指定して初期化)");
}

OB_DEFINE_CLASS_INFO(ob::core::IntVec4) {
	desc("4次元整数ベクトル");
	//tag("C#", "Common.Math.Vector4");
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

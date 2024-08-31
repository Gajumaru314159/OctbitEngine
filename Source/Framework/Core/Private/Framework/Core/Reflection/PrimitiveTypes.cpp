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
				ctor.invoker = [](Span<AnyReference> args) { return Any(std::make_unique<T>()); };
			}

			// コンストラクタ登録(初期値あり)
			{
				auto& ctor = m_info.constructors.emplace_back();
				ctor.arguments = { {Type::Get<T>(),"value"} };
				ctor.invoker = [](Span<AnyReference> args) { return Any(std::make_unique<T>(args[0].get<T>())); };
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
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::u64) {
	desc("64bit符号なし整数型");
	tag("DisplayName", "u64");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::s32) {
	desc("32bit符号付き整数型");
	tag("DisplayName", "s32");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::u32) {
	desc("32bit符号なし整数型");
	tag("DisplayName", "u32");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::s16) {
	desc("16bit符号付き整数型");
	tag("DisplayName", "s16");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::u16) {
	desc("16bit符号なし整数型");
	tag("DisplayName", "u16");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::s8) {
	desc("8bit符号付き整数型");
	tag("DisplayName", "s8");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::u8) {
	desc("8bit符号なし整数型");
	tag("DisplayName", "u8");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::f64) {
	tag("DisplayName", "f64");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::f32) {
	tag("DisplayName", "f32");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::Char) {
	desc("文字型");
}

OB_DEFINE_PRIMITIVE_INFO(ob::core::Bool) {
	desc("真偽値型");
}

OB_DEFINE_CLASS_INFO(ob::core::String) {
	desc("文字列型");
	tag("DisplayName", "ob::core::String");
	constructor<const Char*>().desc("コンストラクタ");
	constructor<String>().desc("コンストラクタ");
}

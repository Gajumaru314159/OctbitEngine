//***********************************************************
//! @file
//! @brief		デバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/IDevice.h>
//! @cond

#define GRAPHIC_DECLEAR_GET_IMPL(type)																	\
	public:																								\
    template<class T>																					\
    static auto GetImpl(const type& obj)->std::enable_if_t<std::is_base_of_v<I##type, T>, const T&> {	\
        return *reinterpret_cast<const T*>(GetImpl(obj));												\
    }       																							\
	private:

#define GRAPHIC_DECLEAR_GET_IMPL_EX(type,base)															\
	public:																								\
    template<class T>																					\
    static auto GetImpl(const base& obj)->std::enable_if_t<std::is_base_of_v<I##type, T>, const T&> {	\
        return *reinterpret_cast<const T*>(GetImpl(obj));												\
    }       																							\
	private:

namespace ob::rhi {

	//@―---------------------------------------------------------------------------
	//! @brief      デバイス
	//! 
	//! @details    デバイスは内部でのみ使用されるクラスです。
	//!             よって、System から直接取得できないようフレンドクラス指定された
	//!             このクラスから取得します。
	//@―---------------------------------------------------------------------------
	class Device {
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  デバイスを取得
		//@―---------------------------------------------------------------------------
		static IDevice* Get();


	private:

		//@―---------------------------------------------------------------------------
		//! @brief  Display の実装を取得
		//@―---------------------------------------------------------------------------
		static const IDisplay* GetImpl(const Display&);
		GRAPHIC_DECLEAR_GET_IMPL(Display);


		//@―---------------------------------------------------------------------------
		//! @brief  CommandList の実装を取得
		//@―---------------------------------------------------------------------------
		static const ICommandList* GetImpl(const CommandList&);
		GRAPHIC_DECLEAR_GET_IMPL(CommandList);


		//@―---------------------------------------------------------------------------
		//! @brief  RenderPass の実装を取得
		//@―---------------------------------------------------------------------------
		static const IRenderPass* GetImpl(const RenderPass&);
		GRAPHIC_DECLEAR_GET_IMPL(RenderPass);


		//@―---------------------------------------------------------------------------
		//! @brief  FrameBuffer の実装を取得
		//@―---------------------------------------------------------------------------
		static const IFrameBuffer* GetImpl(const FrameBuffer&);
		GRAPHIC_DECLEAR_GET_IMPL(FrameBuffer);


		//@―---------------------------------------------------------------------------
		//! @brief  RootSignature の実装を取得
		//@―---------------------------------------------------------------------------
		static const IRootSignature* GetImpl(const RootSignature&);
		GRAPHIC_DECLEAR_GET_IMPL(RootSignature);


		//@―---------------------------------------------------------------------------
		//! @brief  PipelineState の実装を取得
		//@―---------------------------------------------------------------------------
		static const IPipelineState* GetImpl(const PipelineState&);
		GRAPHIC_DECLEAR_GET_IMPL(PipelineState);


		//@―---------------------------------------------------------------------------
		//! @brief  Shader の実装を取得
		//@―---------------------------------------------------------------------------
		static const IShader* GetImpl(const Shader&);
		GRAPHIC_DECLEAR_GET_IMPL_EX(Shader, VertexShader);
		GRAPHIC_DECLEAR_GET_IMPL_EX(Shader, PixelShader);
		GRAPHIC_DECLEAR_GET_IMPL_EX(Shader, GeometryShader);
		GRAPHIC_DECLEAR_GET_IMPL_EX(Shader, HullShader);
		GRAPHIC_DECLEAR_GET_IMPL_EX(Shader, DomainShader);


		//@―---------------------------------------------------------------------------
		//! @brief  Texture の実装を取得
		//@―---------------------------------------------------------------------------
		static const ITexture* GetImpl(const Texture&);
		GRAPHIC_DECLEAR_GET_IMPL(Texture);


		//@―---------------------------------------------------------------------------
		//! @brief  RenderTexture の実装を取得
		//@―---------------------------------------------------------------------------
		static const IRenderTexture* GetImpl(const RenderTexture&);
		GRAPHIC_DECLEAR_GET_IMPL(RenderTexture);


		//@―---------------------------------------------------------------------------
		//! @brief  Buffer の実装を取得
		//@―---------------------------------------------------------------------------
		static const IBuffer* GetImpl(const Buffer&);
		GRAPHIC_DECLEAR_GET_IMPL(Buffer);


		//@―---------------------------------------------------------------------------
		//! @brief  Buffer の実装を取得
		//@―---------------------------------------------------------------------------
		static const IDescriptorTable* GetImpl(const DescriptorTable&);
		GRAPHIC_DECLEAR_GET_IMPL(DescriptorTable);
	};

}// namespcae ob

#undef GRAPHIC_DECLEAR_GET_IMPL
#undef GRAPHIC_DECLEAR_GET_IMPL_EX
//! @endcond
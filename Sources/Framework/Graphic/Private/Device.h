//***********************************************************
//! @file
//! @brief		デバイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/IDevice.h>

namespace ob::graphic {
	class SwapChain;
	class ISwapChain;
	class RootSignature;
	class IRootSignature;
	class PipelineState;
	class IPipelineState;
	class RenderTexture;
	class IRenderTexture;
	class Texture;
	class ITexture;
	class Shader;
	class IShader;
	class VertexShader;
	class PixelShader;
	class GeometryShader;
	class HullShader;
	class DomainShader;
}

#define DECLEAR_GRAPHIC_GET_IMPL(type)														\
    template<class T>																		\
    static auto GetImpl(const type& obj)->enable_if_t<is_base_of_v<I##type, T>, const T*> { \
        return reinterpret_cast<const T*>(GetImpl(obj));									\
    }                                                                             

namespace ob::graphic {

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


	public:

		//@―---------------------------------------------------------------------------
		//! @brief  SwapChain の実装を取得
		//@―---------------------------------------------------------------------------
		static const ISwapChain* GetImpl(const SwapChain&);
		DECLEAR_GRAPHIC_GET_IMPL(SwapChain);


		//@―---------------------------------------------------------------------------
		//! @brief  RootSignature の実装を取得
		//@―---------------------------------------------------------------------------
		static const IRootSignature* GetImpl(const RootSignature&);
		DECLEAR_GRAPHIC_GET_IMPL(RootSignature);


		//@―---------------------------------------------------------------------------
		//! @brief  PipelineState の実装を取得
		//@―---------------------------------------------------------------------------
		static const IPipelineState* GetImpl(const PipelineState&);
		DECLEAR_GRAPHIC_GET_IMPL(PipelineState);


		//@―---------------------------------------------------------------------------
		//! @brief  Shader の実装を取得
		//@―---------------------------------------------------------------------------
		static const IShader* GetImpl(const Shader&);
		template<class T>
		static auto GetImpl(const VertexShader& obj)->const T* {
			return reinterpret_cast<const T*>(GetImpl(obj));
		}
		template<class T>
		static auto GetImpl(const PixelShader& obj)->const T* {
			return reinterpret_cast<const T*>(GetImpl(obj));
		}
		template<class T>
		static auto GetImpl(const GeometryShader& obj)->const T* {
			return reinterpret_cast<const T*>(GetImpl(obj));
		}
		template<class T>
		static auto GetImpl(const HullShader& obj)->const T* {
			return reinterpret_cast<const T*>(GetImpl(obj));
		}
		template<class T>
		static auto GetImpl(const DomainShader& obj)->const T* {
			return reinterpret_cast<const T*>(GetImpl(obj));
		}


		//@―---------------------------------------------------------------------------
		//! @brief  Texture の実装を取得
		//@―---------------------------------------------------------------------------
		static const ITexture* GetImpl(const Texture&);
		DECLEAR_GRAPHIC_GET_IMPL(Texture);


		//@―---------------------------------------------------------------------------
		//! @brief  RenderTexture の実装を取得
		//@―---------------------------------------------------------------------------
		static const IRenderTexture* GetImpl(const RenderTexture&);
		DECLEAR_GRAPHIC_GET_IMPL(RenderTexture);
	};

}// namespcae ob
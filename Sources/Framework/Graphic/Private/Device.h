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
    class RenderTexture;
    class IRenderTexture;
    class Texture;
    class ITexture;
    class Shader;
    class IShader;
    class VertexShader;
    class IVertexShader;
    class PixelShader;
    class IPixelShader;
}

#define DECLEAR_GRAPHIC_GET_IMPL(type)                                            \
    template<class T>                                                             \
    static auto GetImpl(type& obj)->enable_if_t<is_base_of_v<I##type, T>, T*> {  \
        return reinterpret_cast<T*>(GetImpl(obj));                                \
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
        static ISwapChain* GetImpl(SwapChain&);
        DECLEAR_GRAPHIC_GET_IMPL(SwapChain);


        //@―---------------------------------------------------------------------------
        //! @brief  Texture の実装を取得
        //@―---------------------------------------------------------------------------
        static ITexture* GetImpl(Texture&);
        DECLEAR_GRAPHIC_GET_IMPL(Texture);


        //@―---------------------------------------------------------------------------
        //! @brief  RenderTexture の実装を取得
        //@―---------------------------------------------------------------------------
        static IRenderTexture* GetImpl(RenderTexture&);
        DECLEAR_GRAPHIC_GET_IMPL(RenderTexture);


        //@―---------------------------------------------------------------------------
        //! @brief  Shader の実装を取得
        //@―---------------------------------------------------------------------------
        static IShader* GetImpl(Shader&);
        DECLEAR_GRAPHIC_GET_IMPL(Shader);


    };

}// namespcae ob
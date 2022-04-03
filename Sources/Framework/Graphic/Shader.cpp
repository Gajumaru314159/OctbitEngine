//***********************************************************
//! @file
//! @brief		シェーダ
//! @author		Gajumaru
//***********************************************************
#include "Shader.h"
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/Interface/IShader.h>

namespace ob::graphic {

    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(Shader);


    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(VertexShader);


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //@―---------------------------------------------------------------------------
    VertexShader::VertexShader() {

    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param codeSet      シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    VertexShader::VertexShader(const String& code, StringView name = TC("Shader")) {
        m_pImpl = Device::Get()->createShader(code,ShaderType::Vertex, name);
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    VertexShader::VertexShader(const Blob& binary, StringView name = TC("Shader")) {
        m_pImpl = Device::Get()->createShader(binary, ShaderType::Vertex, name);
    }


    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(PixelShader);


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //@―---------------------------------------------------------------------------
    PixelShader::PixelShader() {

    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param codeSet      シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    PixelShader::PixelShader(const String& code, StringView name = TC("Shader")) {
        m_pImpl = Device::Get()->createShader(code, ShaderType::Pixel, name);
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    PixelShader::PixelShader(const Blob& binary, StringView name = TC("Shader")) {
        m_pImpl = Device::Get()->createShader(binary, ShaderType::Pixel, name);
    }

}// namespace ob::graphic
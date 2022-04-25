//***********************************************************
//! @file
//! @brief		シェーダ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphic/Shader.h>
#include <Framework/Graphic/Private/Device.h>
#include <Framework/Graphic/Interface/IShader.h>

namespace ob::graphic {

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のShaderへアクセス")

    //@―---------------------------------------------------------------------------
    //! @brief      シェーダのエントリ関数名を取得
    //! 
    //! @details    無効な値の場合は空白文字列を返します。
    //@―---------------------------------------------------------------------------
    const char* Shader::getEntryName(ShaderStage stage) {
        switch (stage)
        {
        case ob::graphic::ShaderStage::Vertex:          return "VS_Main";
        case ob::graphic::ShaderStage::Hull:            return "HS_Main";
        case ob::graphic::ShaderStage::Domain:          return "DS_Main";
        case ob::graphic::ShaderStage::Geometry:        return "GS_Main";
        case ob::graphic::ShaderStage::Pixel:           return "PS_Main";
        case ob::graphic::ShaderStage::Compute:         return "CS_Main";
        case ob::graphic::ShaderStage::Task:            return "TS_Main";
        case ob::graphic::ShaderStage::Mesh:            return "MS_Main";
        case ob::graphic::ShaderStage::RayGen:          return "RGS_Main";
        case ob::graphic::ShaderStage::AnyHit:          return "AHS_Main";
        case ob::graphic::ShaderStage::ClosestHit:      return "CHS_Main";
        case ob::graphic::ShaderStage::Miss:            return "MS_Main";
        case ob::graphic::ShaderStage::Intersection:    return "IS_Main";
        case ob::graphic::ShaderStage::Callable:        return "CS_Main";
        case ob::graphic::ShaderStage::Amplification:   return "AS_Main";
        }
        return "";
    }


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
    VertexShader::VertexShader(const String& code, StringView name) {
        m_pImpl = Device::Get()->createShader(code,ShaderStage::Vertex);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        m_pImpl->setName(name);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "シェーダの生成に失敗[name={}]", name);
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    VertexShader::VertexShader(const Blob& binary, StringView name) {
        m_pImpl = Device::Get()->createShader(binary, ShaderStage::Vertex);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        m_pImpl->setName(name);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "シェーダの生成に失敗[name={}]", name);
            release();
        }
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
    PixelShader::PixelShader(const String& code, StringView name) {
        m_pImpl = Device::Get()->createShader(code, ShaderStage::Pixel);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        m_pImpl->setName(name);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "シェーダの生成に失敗[name={}]", name);
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    PixelShader::PixelShader(const Blob& binary, StringView name) {
        m_pImpl = Device::Get()->createShader(binary, ShaderStage::Pixel);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        m_pImpl->setName(name);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "シェーダの生成に失敗[name={}]", name);
            release();
        }
    }


    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(GeometryShader);


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //@―---------------------------------------------------------------------------
    GeometryShader::GeometryShader() {

    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param codeSet      シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    GeometryShader::GeometryShader(const String& code, StringView name) {
        m_pImpl = Device::Get()->createShader(code, ShaderStage::Geometry);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        m_pImpl->setName(name);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "シェーダの生成に失敗[name={}]", name);
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    GeometryShader::GeometryShader(const Blob& binary, StringView name) {
        m_pImpl = Device::Get()->createShader(binary, ShaderStage::Geometry);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        m_pImpl->setName(name);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "シェーダの生成に失敗[name={}]", name);
            release();
        }
    }


    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(HullShader);


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //@―---------------------------------------------------------------------------
    HullShader::HullShader() {

    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param codeSet      シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    HullShader::HullShader(const String& code, StringView name) {
        m_pImpl = Device::Get()->createShader(code, ShaderStage::Hull);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        m_pImpl->setName(name);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "シェーダの生成に失敗[name={}]", name);
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    HullShader::HullShader(const Blob& binary, StringView name) {
        m_pImpl = Device::Get()->createShader(binary, ShaderStage::Hull);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        m_pImpl->setName(name);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "シェーダの生成に失敗[name={}]", name);
            release();
        }
    }


    OB_IMPLEMENT_GRAPHIC_OBJECT_HOLDER(DomainShader);


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //@―---------------------------------------------------------------------------
    DomainShader::DomainShader() {

    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param codeSet      シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    DomainShader::DomainShader(const String& code, StringView name) {
        m_pImpl = Device::Get()->createShader(code, ShaderStage::Domain);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        m_pImpl->setName(name);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "シェーダの生成に失敗[name={}]", name);
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    DomainShader::DomainShader(const Blob& binary, StringView name) {
        m_pImpl = Device::Get()->createShader(binary, ShaderStage::Domain);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        m_pImpl->setName(name);
        if (!m_pImpl->isValid()) {
            LOG_ERROR_EX("Graphic", "シェーダの生成に失敗[name={}]", name);
            release();
        }
    }

}// namespace ob::graphic
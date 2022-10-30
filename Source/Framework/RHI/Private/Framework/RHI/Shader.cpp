//***********************************************************
//! @file
//! @brief		シェーダ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Shader.h>
#include <Framework/RHI/Device.h>
#include <Framework/RHI/IShader.h>

namespace ob::rhi {

#define CHECK_IMPL() OB_CHECK_ASSERT(m_pImpl,"未初期化のShaderへアクセス")

    //@―---------------------------------------------------------------------------
    //! @brief      シェーダのエントリ関数名を取得
    //! 
    //! @details    無効な値の場合は空白文字列を返します。
    //@―---------------------------------------------------------------------------
    const char* Shader::getEntryName(ShaderStage stage) {
        switch (stage)
        {
        case ob::rhi::ShaderStage::Vertex:          return "VS_Main";
        case ob::rhi::ShaderStage::Hull:            return "HS_Main";
        case ob::rhi::ShaderStage::Domain:          return "DS_Main";
        case ob::rhi::ShaderStage::Geometry:        return "GS_Main";
        case ob::rhi::ShaderStage::Pixel:           return "PS_Main";
        case ob::rhi::ShaderStage::Compute:         return "CS_Main";
        case ob::rhi::ShaderStage::Task:            return "TS_Main";
        case ob::rhi::ShaderStage::Mesh:            return "MS_Main";
        case ob::rhi::ShaderStage::RayGen:          return "RGS_Main";
        case ob::rhi::ShaderStage::AnyHit:          return "AHS_Main";
        case ob::rhi::ShaderStage::ClosestHit:      return "CHS_Main";
        case ob::rhi::ShaderStage::Miss:            return "MS_Main";
        case ob::rhi::ShaderStage::Intersection:    return "IS_Main";
        case ob::rhi::ShaderStage::Callable:        return "CS_Main";
        case ob::rhi::ShaderStage::Amplification:   return "AS_Main";
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
    VertexShader::VertexShader(const String& code) {
        m_pImpl = Device::Get()->createShader(code,ShaderStage::Vertex);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "シェーダの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    VertexShader::VertexShader(const Blob& binary) {
        m_pImpl = Device::Get()->createShader(binary, ShaderStage::Vertex);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "シェーダの生成に失敗");
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
    PixelShader::PixelShader(const String& code) {
        m_pImpl = Device::Get()->createShader(code, ShaderStage::Pixel);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "シェーダの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    PixelShader::PixelShader(const Blob& binary) {
        m_pImpl = Device::Get()->createShader(binary, ShaderStage::Pixel);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "シェーダの生成に失敗");
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
    GeometryShader::GeometryShader(const String& code) {
        m_pImpl = Device::Get()->createShader(code, ShaderStage::Geometry);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "シェーダの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    GeometryShader::GeometryShader(const Blob& binary) {
        m_pImpl = Device::Get()->createShader(binary, ShaderStage::Geometry);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "シェーダの生成に失敗");
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
    HullShader::HullShader(const String& code) {
        m_pImpl = Device::Get()->createShader(code, ShaderStage::Hull);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "シェーダの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    HullShader::HullShader(const Blob& binary) {
        m_pImpl = Device::Get()->createShader(binary, ShaderStage::Hull);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "シェーダの生成に失敗");
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
    DomainShader::DomainShader(const String& code) {
        m_pImpl = Device::Get()->createShader(code, ShaderStage::Domain);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "シェーダの生成に失敗");
            release();
        }
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    DomainShader::DomainShader(const Blob& binary) {
        m_pImpl = Device::Get()->createShader(binary, ShaderStage::Domain);
        OB_CHECK_ASSERT_EXPR(m_pImpl);
        if (!m_pImpl->isValid()) {
            LOG_FATAL_EX("Graphic", "シェーダの生成に失敗");
            release();
        }
    }

}// namespace ob::rhi
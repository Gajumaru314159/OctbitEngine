//***********************************************************
//! @file
//! @brief		シェーダ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Shader.h>
#include <Framework/RHI/Device.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      シェーダのエントリ関数名を取得
    //! 
    //! @details    無効な値の場合は空白文字列を返します。
    //@―---------------------------------------------------------------------------
    const char* Shader::GetEntryName(ShaderStage stage) {
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

    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param codeSet      シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    Ref<Shader> VertexShader::Create(const String& code) {
        if (auto device = Device::Get()) {
            return device->createShader(code, ShaderStage::Vertex);
        }
        return nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    Ref<Shader> VertexShader::Create(BlobView binary) {
        if (auto device = Device::Get()) {
            return device->createShader(binary, ShaderStage::Vertex);
        }
        return nullptr;
    }
    

    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param codeSet      シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    Ref<Shader> PixelShader::Create(const String& code) {
        if (auto device = Device::Get()) {
            return device->createShader(code, ShaderStage::Pixel);
        }
        return nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param name         オブジェクト名
    //@―---------------------------------------------------------------------------
    Ref<Shader> PixelShader::Create(BlobView binary) {
        if (auto device = Device::Get()) {
            return device->createShader(binary, ShaderStage::Pixel);
        }
        return nullptr;
    }

}// namespace ob::rhi
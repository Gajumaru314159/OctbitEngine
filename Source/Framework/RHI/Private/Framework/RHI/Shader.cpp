//***********************************************************
//! @file
//! @brief		シェーダ
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Shader.h>
#include <Framework/RHI/RHI.h>

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
    //! @param stage        シェーダステージ
    //@―---------------------------------------------------------------------------
    Ref<Shader> Shader::Compile(const String& code,ShaderStage stage) {
        if (auto rhi = RHI::Get()) {
            return rhi->compileShader(code, stage);
        }
        return nullptr;
    }
    Ref<Shader> Shader::CompileVS(const String& code) {
        return Shader::Compile(code, ShaderStage::Vertex);
    }
    Ref<Shader> Shader::CompilePS(const String& code) {
        return Shader::Compile(code, ShaderStage::Pixel);
    }


    //@―---------------------------------------------------------------------------
    //! @brief              コンストラクタ
    //! 
    //! @param binarySet    シェーダ・バイナリ
    //! @param stage        シェーダステージ
    //@―---------------------------------------------------------------------------
    Ref<Shader> Shader::Load(BlobView binary, ShaderStage stage) {
        if (auto rhi = RHI::Get()) {
            return rhi->loadShader(binary, stage);
        }
        return nullptr;
    }
    Ref<Shader> Shader::LoadVS(BlobView binary) {
        return Shader::Load(binary, ShaderStage::Vertex);
    }
    Ref<Shader> Shader::LoadPS(BlobView binary) {
        return Shader::Load(binary, ShaderStage::Vertex);
    }

}
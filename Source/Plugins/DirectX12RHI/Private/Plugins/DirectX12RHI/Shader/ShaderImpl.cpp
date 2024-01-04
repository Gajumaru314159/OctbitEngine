//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "ShaderImpl.h"
#include <d3dcompiler.h>
#include <Framework/Core/String/StringEncoder.h>
#include <Framework/Core/Misc/Blob.h>
#include <Framework/RHI/Shader.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>

namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  ターゲット
    //@―---------------------------------------------------------------------------
    static const char s_shaderTargets[][16] =
    {
        "vs_5_0",
        "ps_5_0"
    };

    //@―---------------------------------------------------------------------------
    //! @brief      シェーダのターゲット名を取得
    //! 
    //! @details    無効な値の場合は空白文字列を返します。
    //@―---------------------------------------------------------------------------
    const char* getTargetName(ShaderStage stage) {
        switch (stage)
        {
        case ShaderStage::Vertex:          return "vs_5_0";
        case ShaderStage::Hull:            return "hs_5_0";
        case ShaderStage::Domain:          return "ds_5_0";
        case ShaderStage::Geometry:        return "gs_5_0";
        case ShaderStage::Pixel:           return "ps_5_0";
        case ShaderStage::Compute:         return "cs_5_0";

        //case ob::rhi::ShaderStage::Task:            return "vs_5_0";
        //case ob::rhi::ShaderStage::Mesh:            return "vs_5_0";
        //case ob::rhi::ShaderStage::RayGen:          return "vs_5_0";
        //case ob::rhi::ShaderStage::AnyHit:          return "vs_5_0";
        //case ob::rhi::ShaderStage::ClosestHit:      return "vs_5_0";
        //case ob::rhi::ShaderStage::Miss:            return "vs_5_0";
        //case ob::rhi::ShaderStage::Intersection:    return "vs_5_0";
        //case ob::rhi::ShaderStage::Callable:        return "vs_5_0";
        //case ob::rhi::ShaderStage::Amplification:   return "vs_5_0";
        }
        return "";
    }


    //@―---------------------------------------------------------------------------
    //! @brief				シェーダーコードからシェーダーオブジェクトを生成
    //!
    //! @param src			シェーダコード
    //! @param stage		シェーダステージ
    //! @param errorDest	エラー出力先文字列
    //@―---------------------------------------------------------------------------
    ShaderImpl::ShaderImpl(const String& code, ShaderStage stage, StringView name)
        : m_name(name)
    {
        // コンパイルできるようにUTF-8にコンバート
        StringBase<char> utfCode;
        StringEncoder::Encode(code, utfCode);

        compile(utfCode, stage);

    }



    //@―---------------------------------------------------------------------------
    //! @brief				バイナリからシェーダーオブジェクトを生成
    //!
    //! @param src			バイナリ
    //! @param stage		シェーダステージ
    //! @param errorDest	エラー出力先文字列
    //@―---------------------------------------------------------------------------
    ShaderImpl::ShaderImpl(BlobView blob, ShaderStage stage, StringView name)
        : m_name(name)
    {
        // チャンクタイプをチェック
        bool isShaderBlob = (!blob.empty()) && *reinterpret_cast<const u32*>(blob.data()) == 0x43425844;
        if (!isShaderBlob) {
            LOG_ERROR_EX("Graphic", "シェーダではないバイナリファイルから構築しようとしました。");
        }
        m_shaderBlob = Blob(blob.data(),blob.size());
    }


    //@―---------------------------------------------------------------------------
    //! @brief              妥当な状態か
    //@―---------------------------------------------------------------------------
    bool ShaderImpl::isValid()const {
        if (!m_shaderBlob.empty())return true;
        if (m_shaderBolb2)return true;
        return false;
    }

    
    //@―---------------------------------------------------------------------------
    //! @brief      名前を取得
    //@―---------------------------------------------------------------------------
    const String& ShaderImpl::getName()const {
        return m_name;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  シェーダステージを取得
    //@―---------------------------------------------------------------------------
    ShaderStage ShaderImpl::getStage()const {
        return m_stage;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バイナリデータを取得
    //@―---------------------------------------------------------------------------
    const void* ShaderImpl::getBinaryData()const {
        if (!m_shaderBlob.empty())return m_shaderBlob.data();
        if (m_shaderBolb2)return m_shaderBolb2->GetBufferPointer();
        return nullptr;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  バイナリサイズを取得
    //@―---------------------------------------------------------------------------
    SIZE_T ShaderImpl::getBinarySize()const {
        if (!m_shaderBlob.empty())return m_shaderBlob.size();
        if (m_shaderBolb2)return m_shaderBolb2->GetBufferSize();
        return 0;
    }


    //@―---------------------------------------------------------------------------
    //! @brief				初期化
    //@―---------------------------------------------------------------------------
    void ShaderImpl::compile(const StringBase<char>& code, ShaderStage stage) {

        ComPtr<ID3DBlob> errorBlob;

        // シェーダコードをコンパイル
        auto result = D3DCompile(
            code.data(), code.size(),
            NULL,                           // ソース名
            NULL,                           // Define
            NULL,                           // インクルード
            Shader::GetEntryName(stage),    // エントリ
            getTargetName(stage),           // ターゲット
            D3DCOMPILE_PREFER_FLOW_CONTROL, // フラグ1
            0,                              // フラグ2
            m_shaderBolb2.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());

        if (FAILED(result)) {
            Utility::OutputErrorLog(result, TC("シェーダコンパイルエラー"));

            // エラー内容取得
            StringBase<char> error;
            error.resize(errorBlob->GetBufferSize());
            copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), error.begin());

            // 変換
            String errorDest;
            StringEncoder::Encode(error, errorDest);
            LOG_ERROR_EX("Graphic", "{}", errorDest);

            return;
        }
    }

}
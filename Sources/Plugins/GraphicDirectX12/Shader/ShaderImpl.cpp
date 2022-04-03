//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include "ShaderImpl.h"
#include <d3dcompiler.h>
#include <Framework/Foundation/String/StringEncoder.h>

namespace ob::graphic::dx12 {

    static const char s_shaderTargets[][16] =
    {
        "vs_5_0",
        "ps_5_0"
    };
    static const char s_entryName[][16] =
    {
        "VSMain",
        "PSMain"
    };


    //@―---------------------------------------------------------------------------
    //! @brief				シェーダーコードからシェーダーオブジェクトを生成
    //!
    //! @param src			シェーダコード
    //! @param type			シェーダタイプ
    //! @param errorDest	エラー出力先文字列
    //@―---------------------------------------------------------------------------
    ShaderImpl::ShaderImpl(const String& code, const ShaderType type, StringView name)
        : IShader(name) {

        // コンパイルできるようにUTF-8にコンバート
        StringBase<char> utfCode;
        StringEncoder::Encode(code, utfCode);

        Blob blob(utfCode.data(),utfCode.size());

        initialize(blob,type);
    }



    //@―---------------------------------------------------------------------------
    //! @brief				バイナリからシェーダーオブジェクトを生成
    //!
    //! @param src			バイナリ
    //! @param type			シェーダタイプ
    //! @param errorDest	エラー出力先文字列
    //@―---------------------------------------------------------------------------
    ShaderImpl::ShaderImpl(const Blob& blob, const ShaderType type, StringView name)
        : IShader(name) {

    }


    //@―---------------------------------------------------------------------------
    //! @brief				初期化
    //@―---------------------------------------------------------------------------
    void ShaderImpl::initialize(const Blob& blob, const ShaderType type) {

        ComPtr<ID3DBlob> errorBlob = nullptr;

        s32 index = enum_cast(type);
        if (!is_in_range(index, 0, get_size(s_entryName))) {
            LOG_ERROR("不正なシェーダタイプ");
            return;
        }

        // チャンクタイプをチェック
        bool isCompiled = *reinterpret_cast<const u32*>(blob.data()) == 0x43425844;

        if (isCompiled) {

            // バイナリデータから直接構築
            OB_NOTIMPLEMENTED();

        } else {

            // シェーダコードをコンパイル
            auto result = D3DCompile(blob.data(), blob.size(), NULL, NULL, NULL,
                s_entryName[index], s_shaderTargets[index],
                D3DCOMPILE_PREFER_FLOW_CONTROL, 0,
                m_shaderBolb.ReleaseAndGetAddressOf(), errorBlob.ReleaseAndGetAddressOf());

            // コンパイルに失敗したらエラーを出力
            if (FAILED(result)) {

                StringBase<char> error;
                error.resize(errorBlob->GetBufferSize());
                copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), error.begin());

                String errorDest;
                StringEncoder::Encode(error, errorDest);
                LOG_ERROR("{}", errorDest.c_str());

                return;
            }

        }
    }

}// namespace ob::graphic::dx12
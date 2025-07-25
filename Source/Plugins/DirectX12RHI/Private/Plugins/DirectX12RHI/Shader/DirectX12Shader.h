//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Shader.h>
#include <Framework/Core/Misc/Blob.h>
#include <Framework/RHI/Types/PipelineStateDesc.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi {
    class DirectX12Device;
    class ITexture;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi {

    //! @brief  シェーダ実装(DirectX12)
    class DirectX12Shader :public Shader {
    public:

        //===============================================================
        //	コンストラクタ
        //===============================================================

        //! @brief				シェーダーコードからシェーダーオブジェクトを生成
        DirectX12Shader(DirectX12Device& device, const ShaderCompileDesc& desc);


        //! @brief				バイナリからシェーダーオブジェクトを生成
        //!
        //! @param src			バイナリ
        //! @param stage		シェーダステージ
        //! @param errorDest	エラー出力先文字列
        DirectX12Shader(BlobView blob, ShaderStage stage, StringView name="Shader");


        //! @brief  妥当な状態か
        bool isValid()const;

        
        //! @brief      名前を取得
        const String& getName()const override;


        //! @brief  シェーダステージを取得
        ShaderStage getStage()const override;


        //! @brief  バイナリデータを取得
        const void* getBinaryData()const;


        //! @brief  バイナリサイズを取得
        SIZE_T getBinarySize()const;


    public:

        //! @brief  コンパイル
        void compile(DirectX12Device& device, const ShaderCompileDesc& desc);

    private:

        String              m_name;
        ShaderStage         m_stage;                        //!< シェーダ・ステージ
        Blob                m_shaderBlob;                   //!< シェーダ・バイナリ
        ComPtr<IDxcBlob>    m_shaderBolb2;					//!< シェーダ・バイナリ
        
        Vector<VertexAttribute> m_attributes;

    };

}
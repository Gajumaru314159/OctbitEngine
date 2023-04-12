﻿//***********************************************************
//! @file
//! @brief		シェーダ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Shader.h>
#include <Framework/Core/Misc/Blob.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::rhi::dx12 {
    class DirectX12RHI;
    class ITexture;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  シェーダ実装(DirectX12)
    //@―---------------------------------------------------------------------------
    class ShaderImpl :public Shader {
    public:

        //===============================================================
        //	コンストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief				シェーダーコードからシェーダーオブジェクトを生成
        //!
        //! @param src			シェーダコード
        //! @param stage		シェーダステージ
        //! @param errorDest	エラー出力先文字列
        //@―---------------------------------------------------------------------------
        ShaderImpl(const String& code, ShaderStage stage, StringView name=TC("Shader"));


        //@―---------------------------------------------------------------------------
        //! @brief				バイナリからシェーダーオブジェクトを生成
        //!
        //! @param src			バイナリ
        //! @param stage		シェーダステージ
        //! @param errorDest	エラー出力先文字列
        //@―---------------------------------------------------------------------------
        ShaderImpl(BlobView blob, ShaderStage stage, StringView name=TC("Shader"));


        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        bool isValid()const;

        
        //@―---------------------------------------------------------------------------
        //! @brief      名前を取得
        //@―---------------------------------------------------------------------------
        const String& getName()const override;


        //@―---------------------------------------------------------------------------
        //! @brief  シェーダステージを取得
        //@―---------------------------------------------------------------------------
        ShaderStage getStage()const override;


        //@―---------------------------------------------------------------------------
        //! @brief  バイナリデータを取得
        //@―---------------------------------------------------------------------------
        const void* getBinaryData()const;


        //@―---------------------------------------------------------------------------
        //! @brief  バイナリサイズを取得
        //@―---------------------------------------------------------------------------
        SIZE_T getBinarySize()const;


    public:

        //@―---------------------------------------------------------------------------
        //! @brief  コンパイル
        //@―---------------------------------------------------------------------------
        void compile(const StringBase<char>& blob,ShaderStage stage);


    private:

        String              m_name;
        ShaderStage         m_stage;                        //!< シェーダ・ステージ
        Blob                m_shaderBlob;                   //!< シェーダ・バイナリ
        ComPtr<ID3DBlob>    m_shaderBolb2;					//!< シェーダ・バイナリ

    };

}
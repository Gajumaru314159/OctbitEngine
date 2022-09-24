﻿//***********************************************************
//! @file
//! @brief		シェーダ実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/IShader.h>
#include <Framework/Core/Misc/Blob.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic::dx12 {
    class DeviceImpl;
    class ITexture;
}


//===============================================================
// クラス定義
//===============================================================
namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  シェーダ実装(DirectX12)
    //@―---------------------------------------------------------------------------
    class ShaderImpl :public IShader {
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
        ShaderImpl(const String& code, ShaderStage stage);


        //@―---------------------------------------------------------------------------
        //! @brief				バイナリからシェーダーオブジェクトを生成
        //!
        //! @param src			バイナリ
        //! @param stage		シェーダステージ
        //! @param errorDest	エラー出力先文字列
        //@―---------------------------------------------------------------------------
        ShaderImpl(const Blob& blob, ShaderStage stage);


        //@―---------------------------------------------------------------------------
        //! @brief  妥当な状態か
        //@―---------------------------------------------------------------------------
        bool isValid()const;


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

        ShaderStage         m_stage;                        //!< シェーダ・ステージ
        Blob                m_shaderBlob;                   //!< シェーダ・バイナリ
        ComPtr<ID3DBlob>    m_shaderBolb2;					//!< シェーダ・バイナリ

    };

}// namespcae ob::graphic::dx12
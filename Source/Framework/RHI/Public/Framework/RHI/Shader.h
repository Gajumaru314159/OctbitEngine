﻿//***********************************************************
//! @file
//! @brief		シェーダ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/ShaderStage.h>

//===============================================================
// クラス定義
//===============================================================
namespace ob::core{
    class Blob;
}

//===============================================================
// クラス定義
//===============================================================
namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      頂点シェーダ
    //@―---------------------------------------------------------------------------
    class Shader :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief      シェーダのエントリ関数名を取得
        //! 
        //! @details    取得できる関数名は以下の通りです。
        //!             | ステージ     | エントリ名 |
        //!             |--------------|------------|
        //!             | Vertex       | VS_Main    |
        //!             | Hull         | HS_Main    |
        //!             | Domain       | DS_Main    |
        //!             | Geometry     | GS_Main    |
        //!             | Pixel        | PS_Main    |
        //!             | Compute      | CS_Main    |
        //!             | Task         | TS_Main    |
        //!             | Mesh         | MS_Main    |
        //!             | RayGen       | RGS_Main   |
        //!             | AnyHit       | AHS_Main   |
        //!             | ClosestHit   | CHS_Main   |
        //!             | Miss         | MS_Main    |
        //!             | Intersection | IS_Main    |
        //!             | Callable     | CS_Main    |
        //!             | Amplification| AS_Main    |
        //!             | 無効値       | 空白文字列 |
        //@―---------------------------------------------------------------------------
        static const char* GetEntryName(ShaderStage stage);

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  シェーダステージを取得
        //@―---------------------------------------------------------------------------
        virtual ShaderStage getStage()const = 0;


    protected:

        virtual ~Shader() = default;

    };

    //@―---------------------------------------------------------------------------
    //! @brief      頂点シェーダ
    //@―---------------------------------------------------------------------------
    class VertexShader:public Shader {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ
        //! 
        //! @param code     シェーダコード
        //! @param name     オブジェクト名
        //@―---------------------------------------------------------------------------
        static Ref<Shader> Create(const String& code);


        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ
        //! 
        //! @param binary   シェーダ・バイナリ
        //! @param name     オブジェクト名
        //@―---------------------------------------------------------------------------
        static Ref<Shader> Create(const Blob& binary);

    };


    //@―---------------------------------------------------------------------------
    //! @brief      シェーダ
    //@―---------------------------------------------------------------------------
    class PixelShader :public Shader {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ
        //! 
        //! @param code     シェーダコード
        //! @param name     オブジェクト名
        //@―---------------------------------------------------------------------------
        static Ref<Shader> Create(const String& code);


        //@―---------------------------------------------------------------------------
        //! @brief          コンストラクタ
        //! 
        //! @param binary   シェーダ・バイナリ
        //! @param name     オブジェクト名
        //@―---------------------------------------------------------------------------
        static Ref<Shader> Create(const Blob& binary);

    };

}// namespace pb::rhi


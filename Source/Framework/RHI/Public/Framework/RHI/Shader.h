//***********************************************************
//! @file
//! @brief		シェーダ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/BlobView.h>
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/ShaderStage.h>

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

        //@―---------------------------------------------------------------------------
        //! @brief          シェーダコードをコンパイル
        //! 
        //! @param code     シェーダコード
        //! @param stage    シェーダステージ
        //@―---------------------------------------------------------------------------
        static Ref<Shader> Compile(const String& code, ShaderStage stage);
        static Ref<Shader> CompileVS(const String& code);
        static Ref<Shader> CompilePS(const String& code);


        //@―---------------------------------------------------------------------------
        //! @brief          シェーダバイナリをロード
        //! 
        //! @param binary   シェーダ・バイナリ
        //! @param stage    シェーダステージ
        //@―---------------------------------------------------------------------------
        static Ref<Shader> Load(BlobView binary, ShaderStage stage);
        static Ref<Shader> LoadVS(BlobView binary);
        static Ref<Shader> LoadPS(BlobView binary);

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  シェーダステージを取得
        //@―---------------------------------------------------------------------------
        virtual ShaderStage getStage()const = 0;


    protected:

        virtual ~Shader() = default;

    };

}// namespace pb::rhi


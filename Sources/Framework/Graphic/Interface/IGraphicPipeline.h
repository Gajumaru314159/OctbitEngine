//***********************************************************
//! @file
//! @brief		グラフィックパイプライン・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/GraphicObject.h>

//===============================================================
// 前方宣言
//===============================================================
namespace ob::graphic {
}

//===============================================================
// クラス宣言
//===============================================================
namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief      グラフィックパイプライン・インターフェイス
    //@―---------------------------------------------------------------------------
    class IGraphicPipeline :public GraphicObject {
    public:

		/// <summary>
		/// コマンドリストにこのグラフィックパイプラインを設定する
		/// </summary>
		/// <param name="commandList">グラフィックコマンドリスト</param>
		/// <returns>　０：成功\n－１：失敗</returns>
		

		//@―---------------------------------------------------------------------------
		//! @brief      グラフィックパイプライン・インターフェイス
		//@―---------------------------------------------------------------------------
		virtual s32 SetGraphicPipeline(ComPtr<ID3D12GraphicsCommandList>& commandList)const;

		/// <summary>
		/// 指定したレジスタの定数バッファのサイズを取得
		/// </summary>
		/// <param name="resister">レジスタ番号</param>
		/// <returns>定数バッファのサイズ</returns>
		virtual s32 GetConstantBufferSize(const s32 resister)const;

		/// <summary>
		/// 指定したレジスタのテクスチャの枚数を取得
		/// </summary>
		///  <param name="resister">レジスタ番号</param>
		/// <returns>枚数</returns>
		virtual s32 GetTextureNum(const s32 resister)const;

		/// <summary>
		/// 指定した定数バッファのレジスタがディスクリプタヒープの何番目に割り当てられているかを取得
		/// </summary>
		/// <param name="resister"></param>
		/// <returns>－１以外：インデックス\n　　－１：指定したレジスタがマテリアルに含まれていない</returns>
		virtual s32 GetConstantBufferIndex(const s32 resister)const;

		/// <summary>
		/// 指定したテクスチャのレジスタがディスクリプタヒープの何番目に割り当てられているかを取得
		/// </summary>
		/// <param name="resister"></param>
		/// <returns>－１以外：インデックス\n　　－１：指定したレジスタがマテリアルに含まれていない</returns>
		virtual s32 GetTextureIndex(const s32 resister)const;

		/// <summary>
		/// シェーダー変数の情報を変数名から取得する
		/// </summary>
		/// <param name="name">変数名</param>
		/// <returns>変数が存在する場合は変数情報を返す。存在しない場合は変数情報のレジスタ番号が-1となる。</returns>
		virtual ShaderVariableDesc GetVariableData(const String& name)const;

		/// <summary>
		/// シェーダー変数の情報を変数名から取得する
		/// </summary>
		/// <param name="name">変数名</param>
		/// <returns>変数が存在する場合は変数情報を返す。存在しない場合は変数情報のレジスタ番号が-1となる。</returns>
		virtual ShaderTextureDesc GetTextureData(const String& name)const;


		/// <summary>
		/// シェーダーに含まれる変数の一覧を取得する
		/// </summary>
		/// <param name="dest">変数データの出力先のリスト</param>
		/// <returns>　０：取得成功\n－１：取得失敗</returns>
		virtual const HashMap<String, ShaderVariableDesc>& GetShaderParamList()const;


		/// <summary>
		/// インスタンスの生成に成功しているか
		/// </summary>
		/// <returns></returns>
		virtual bool IsValid()const { return m_pipelineState != nullptr; }

    
    };

}// namespace pb::graphic
//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Interface/IGraphicPipeline.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------

	// シェーダパラメータ情報
	struct ShaderVariableDesc {
		//ShaderParamType type2;
		D3D_SHADER_VARIABLE_TYPE type;
		s32 registerNum;
		U32 offset;
		U32 rows;
		U32 columns;
		U32 elementCount;
	};
	struct ShaderTextureDesc {
		s32 registerNum;
		U32 dimension;
	};


	/// <summary>
	/// 描画に使用する情報をひとまとめにする
	/// </summary>
	class GraphicPipelineImpl :public IGraphicPipeline {
	public:
		static const s32 MAX_REGISTER = 32;
	public:
		GraphicPipelineImpl(const GraphicPipelineDesc& desc);


		/// <summary>
		/// コマンドリストにこのグラフィックパイプラインを設定する
		/// </summary>
		/// <param name="commandList">グラフィックコマンドリスト</param>
		/// <returns>　０：成功\n－１：失敗</returns>
		s32 SetGraphicPipeline(ComPtr<ID3D12GraphicsCommandList>& commandList)const;

		/// <summary>
		/// 指定したレジスタの定数バッファのサイズを取得
		/// </summary>
		/// <param name="resister">レジスタ番号</param>
		/// <returns>定数バッファのサイズ</returns>
		s32 GetConstantBufferSize(const U32 resister)const;

		/// <summary>
		/// 指定したレジスタのテクスチャの枚数を取得
		/// </summary>
		///  <param name="resister">レジスタ番号</param>
		/// <returns>枚数</returns>
		s32 GetTextureNum(const U32 resister)const;

		/// <summary>
		/// 指定した定数バッファのレジスタがディスクリプタヒープの何番目に割り当てられているかを取得
		/// </summary>
		/// <param name="resister"></param>
		/// <returns>－１以外：インデックス\n　　－１：指定したレジスタがマテリアルに含まれていない</returns>
		s32 GetConstantBufferIndex(const U32 resister)const;

		/// <summary>
		/// 指定したテクスチャのレジスタがディスクリプタヒープの何番目に割り当てられているかを取得
		/// </summary>
		/// <param name="resister"></param>
		/// <returns>－１以外：インデックス\n　　－１：指定したレジスタがマテリアルに含まれていない</returns>
		s32 GetTextureIndex(const U32 resister)const;

		/// <summary>
		/// シェーダー変数の情報を変数名から取得する
		/// </summary>
		/// <param name="name">変数名</param>
		/// <returns>変数が存在する場合は変数情報を返す。存在しない場合は変数情報のレジスタ番号が-1となる。</returns>
		ShaderVariableDesc GetVariableData(const String& name)const;

		/// <summary>
		/// シェーダー変数の情報を変数名から取得する
		/// </summary>
		/// <param name="name">変数名</param>
		/// <returns>変数が存在する場合は変数情報を返す。存在しない場合は変数情報のレジスタ番号が-1となる。</returns>
		ShaderTextureDesc GetTextureData(const String& name)const;


		/// <summary>
		/// シェーダーに含まれる変数の一覧を取得する
		/// </summary>
		/// <param name="dest">変数データの出力先のリスト</param>
		/// <returns>　０：取得成功\n－１：取得失敗</returns>
		const HashMap<String, ShaderVariableDesc>& GetShaderParamList()const;


		/// <summary>
		/// インスタンスの生成に成功しているか
		/// </summary>
		/// <returns></returns>
		inline bool IsValid()const { return m_pipelineState != nullptr; }

	private:
		s32 ReflectShader(const ComPtr<ID3DBlob>& vsInstance);
		s32 ReflectInputLayout(const ComPtr<ID3DBlob>& vsInstance);
		s32 ReflectConstantBuffer(const ComPtr<ID3D12ShaderReflection>& reflection);
		s32 ReflectBoundingResource(const ComPtr<ID3D12ShaderReflection>& reflection);
		s32 ReflectOutputLayout(const ComPtr<ID3DBlob>& vsInstance);

	private:
		// メインリソース
		ComPtr<ID3D12PipelineState> m_pipelineState;
		ComPtr<ID3D12RootSignature> m_rootSignature;

		// シェーダ参照
		SPtr<IShader> m_vs;
		SPtr<IShader> m_ps;
		SPtr<IShader> m_gs;
		SPtr<IShader> m_hs;
		SPtr<IShader> m_ds;



		// 頂点レイアウトの定義
		ArrayList<String> m_inputLayoutNames;
		ArrayList<D3D12_INPUT_ELEMENT_DESC> m_inputLayout;

		// 変数定義マップ
		HashMap<String, ShaderVariableDesc> m_varMap;
		HashMap<String, ShaderTextureDesc> m_texMap;

		// 定数バッファのレジスタごとのサイズ
		U32 m_cBufDataSizes[MAX_REGISTER];
		// テクスチャのレジスタごとの枚数
		U32 m_texNums[MAX_REGISTER];


		s32 m_cBufIndices[MAX_REGISTER];
		s32 m_texIndices[MAX_REGISTER];

		// レンダーターゲットの枚数
		s32 m_targetNum;

		static const HashMap<BlendFactor, D3D12_BLEND> ms_blendFactorMap;

	};






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob::graphic::dx12
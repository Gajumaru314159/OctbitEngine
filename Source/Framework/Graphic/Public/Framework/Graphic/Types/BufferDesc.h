//***********************************************************
//! @file
//! @brief		バッファ定義
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

#pragma region Enum

	//@―---------------------------------------------------------------------------
	//! @brief  リソース使用法
	//@―---------------------------------------------------------------------------
	enum class ResourceUsage {
		Default,    //!< GPUでの読み取り/書き込み
		Immutable,  //!< GPUでの読み取り
		Dynamic,    //!< CPUでの書き込み/GPUでの読み取り
		ReadBack,   //!< GPUでの書き込み/CPUでの読み取り
		WiteBack,   //!< CPUでの書き込み/GPUでの読み取り
	};


	//@―---------------------------------------------------------------------------
	//! @brief  バッファ・タイプ
	//@―---------------------------------------------------------------------------
	enum class BufferType {
		Unknown,                //!< 指定なし
		VertexBuffer,           //!< 頂点バッファ
		IndexBuffer,            //!< インデックスバッファ
		ConstantBuffer,         //!< 定数バッファ
		UnorderedAccess,        //!< アンオーダード・アクセス
		PixelShadeResource,     //!< ピクセルシェーダで利用可能
		ComputeShaderResource,  //!< コンピュートシェーダで利用可能
		AllShaderResource,      //!< 全てのシェーダで利用可能
		StreamOut,              //!< ストリーム出力先
		IndirectArgument,       //!< 間接引数
		CopySource,             //!< コピー元
		CopyDest,               //!< コピー先
	};

#pragma endregion

#pragma region Flag

	//@―---------------------------------------------------------------------------
	//! @brief  バインド・フラグ
	//@―---------------------------------------------------------------------------
	enum class BindFlag {
		NonPixelShaderResource	= get_bit(0),   //!< ピクセルシェーダ以外でのバインド許可
		PixelShaderResource		= get_bit(1),   //!< ピクセルシェーダのバインド許可
		AllShaderResource		= NonPixelShaderResource | PixelShaderResource, //!< 全てのシェーダでバインド許可
		UnorderedAccess			= get_bit(2),   //!< UnorderedAccessのバインド許可
		RenderTarget			= get_bit(3),   //!< レンダーターゲットビューの作成許可
		CopySource				= get_bit(4),   //!< コピー元許可
		CopyDest				= get_bit(5),   //!< コピー先許可
	};
	//@―---------------------------------------------------------------------------
	//! @brief  バインド・フラグ・セット
	//@―---------------------------------------------------------------------------
	using BindFlags = BitFlags<BindFlag>;


	//@―---------------------------------------------------------------------------
	//! @brief  バッファ・フラグ
	//@―---------------------------------------------------------------------------
	enum class BufferFlag {
		IndirectArgs	= get_bit(0), //!< 間接引数として初期化
		ZeroClear		= get_bit(1), //!< 初期化時にゼロクリア
	};
	//@―---------------------------------------------------------------------------
	//! @brief  バッファ・フラグ・セット
	//@―---------------------------------------------------------------------------
	using BufferFlags = BitFlags<BufferFlag>;

#pragma endregion

	//@―---------------------------------------------------------------------------
	//! @brief  バッファ定義
	//@―---------------------------------------------------------------------------
	struct BufferDesc {

		BufferType      bufferType;     //!< バッファタイプ
		ResourceUsage   usage;          //!< リソース使用法
		u64             bufferSize;     //!< バッファサイズ
		u32             bufferStride;   //!< ストライブ幅
		BufferFlags     bufferFlags;    //!< バッファフラグ
		BindFlags       bindFlags;      //!< バインドフラグ

	public:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		BufferDesc() = default;


		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		BufferDesc(
			BufferType      bufferType,
			ResourceUsage   usage,
			u64             bufferSize,
			u32             bufferStride,
			BufferFlags      bufferFlags,
			BindFlags       bindFlags)
			: bufferType(bufferType)
			, usage(usage)
			, bufferSize(bufferSize)
			, bufferStride(bufferStride)
			, bufferFlags(bufferFlags)
			, bindFlags(bindFlags)
		{}

		//@―---------------------------------------------------------------------------
		//! @brief  定数バッファ用初期化
		//@―---------------------------------------------------------------------------
		static BufferDesc Constant(
			u64             bufferSize,
			BindFlags       bindFlags,
			ResourceUsage   usage = ResourceUsage::Dynamic,
			BufferFlags      bufferFlags = {}
		)
		{
			return BufferDesc(
				BufferType::ConstantBuffer,
				usage,
				bufferSize,
				0,
				bufferFlags,
				bindFlags
			);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  頂点バッファ用初期化
		//@―---------------------------------------------------------------------------
		template<typename TVertex>
		static BufferDesc Vertex(
			u64             count,
			BindFlags       bindFlags = {},
			ResourceUsage   usage = ResourceUsage::Dynamic,
			BufferFlags      bufferFlags = {}
		)
		{
			return BufferDesc(
				BufferType::VertexBuffer,
				usage,
				sizeof(TVertex)* count,
				sizeof(TVertex),
				bufferFlags,
				bindFlags
			);
		}

		//@―---------------------------------------------------------------------------
		//! @brief  インデックスバッファ用初期化
		//@―---------------------------------------------------------------------------
		template<typename TIndex>
		static BufferDesc Index(
			u64             count,
			BindFlags       bindFlags = {},
			ResourceUsage   usage = ResourceUsage::Dynamic,
			BufferFlags      bufferFlags = {}
		)
		{
			return BufferDesc(
				BufferType::IndexBuffer,
				usage,
				sizeof(TIndex) * count,
				sizeof(TIndex),
				bufferFlags,
				bindFlags
			);
		}

	};

}// namespcae ob::graphic
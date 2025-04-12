//***********************************************************
//! @file
//! @brief		バッファ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/ResourceState.h>

namespace ob::rhi {

#pragma region Enum

	//! @brief  バッファ・タイプ
	enum class BufferState {
		Unknown,                //!< 指定なし
		Common,					//!< 
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

	//! @brief  バインド・フラグ
	enum class BufferFlag {
		ShaderResource			= get_bit(0),	//!< シェーダでバインド許可
		UnorderedAccess			= get_bit(2),   //!< UnorderedAccessのバインド許可
		CopySource				= get_bit(3),   //!< コピー元許可
		CopyDest				= get_bit(4),   //!< コピー先許可

		Vertex,
		Index,
		Constant,
		IndirectArgument
	};
	//! @brief  バインド・フラグ・セット
	using BufferFlags = BitFlags<BufferFlag>;

#pragma endregion

	//! @brief  バッファ定義
	struct BufferDesc {

		String			name;			//!< 名前
		BufferState		state;			//!< バッファタイプ
		u64             size;			//!< バッファサイズ
		u32             stride;			//!< ストライド幅 StructuredBufferで使用する場合structureのサイズを設定する。
		BufferFlags     flags;			//!< バインドフラグ

	public:

		//! @brief  コンストラクタ
		BufferDesc() = default;


		//! @brief  コンストラクタ
		BufferDesc(
			BufferState     state,
			u64             size,
			u32				stride,
			BufferFlags     flags)
			: state(state)
			, size(size)
			, stride(stride)
			, flags(flags)
		{}

		//! @brief  定数バッファ用初期化
		//! @details サイズが256の倍数になるように調整されます。
		static BufferDesc Constant(
			u64				size,
			BufferFlags     flags = BufferFlag::ShaderResource
		)
		{
			return BufferDesc(
				BufferState::ConstantBuffer,
				align_up(size,256),
				0,
				flags
			);
		}

		//! @brief  頂点バッファ用初期化
		template<typename TVertex>
		static BufferDesc Vertex(
			u64             count,
			BufferFlags     flags = {}
		)
		{
			return BufferDesc(
				BufferState::VertexBuffer,
				sizeof(TVertex)* count,
				sizeof(TVertex),
				flags
			);
		}

		//! @brief  インデックスバッファ用初期化
		template<typename TIndex>
		static BufferDesc Index(
			u64             count,
			BufferFlags     flags = {}
		)
		{
			return BufferDesc(
				BufferState::IndexBuffer,
				sizeof(TIndex) * count,
				sizeof(TIndex),
				flags
			);
		}

	};

}
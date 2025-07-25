//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Forward.h>

namespace ob::rhi {

	//! @brief      バッファ状態
	//! 
	//! @details    D3D12_RESOURCE_STATE VkImageLayout VkAccessFlagBits参照
	//!             | ResourceState                     | D3D12_RESOURCE_STATE              | VkAccessFlagBits              |
	//!             |-----------------------------------|-----------------------------------|-------------------------------|
    //!             | Common                            | COMMON                            | -                             |
	//!             | VertexBuffer                      | VERTEX_AND_CONSTANT_BUFFER        | VERTEX_ATTRIBUTE_READ         |
	//!             | IndexBuffer                       | INDEX_BUFFER                      | INDEX_READ                    |
	//!             | ConstantBuffer                    | VERTEX_AND_CONSTANT_BUFFER        | UNIFORM_READ                  |
	//!             | ShadeResource                     | ALL_SHADER_RESOURCE               | SHADER_READ                   |
	//!             | UnorderedAccess                   | UNORDERED_ACCESS                  | SHADER_WRITE                  |
	//!             | IndirectArgument                  | INDIRECT_ARGUMENT                 | INDIRECT_COMMAND_READ         |
	//!             | CopyDest                          | COPY_DEST                         | TRANSFER_SRC_OPTIMAL          |
	//!             | CopySource                        | COPY_SOURCE                       | TRANSFER_DST_OPTIMAL          |
	enum class BufferState {
		Common,					//!< 
		Vertex,					//!< 頂点
		Index,					//!< インデックス
		Constant,				//!< 定数
		ShaderResource,			//!< シェーダリソース (読み取りリソース)
		UnorderedAccess,        //!< アンオーダード・アクセス (読み書きリソース)
		IndirectArgument,       //!< 間接引数
		CopySource,             //!< コピー元
		CopyDest,               //!< コピー先
	};

	//! @brief  バインド・フラグ
	enum class BufferFlag {
		Vertex				= get_bit(0),	//!< 頂点バッファ許可
		Index				= get_bit(1),	//!< インデックスバッファ許可
		Constant			= get_bit(2),	//!< 定数バッファ許可
		ShaderResource		= get_bit(3),	//!< シェーダでバインド許可
		UnorderedAccess		= get_bit(4),   //!< UnorderedAccessのバインド許可
		CopySource			= get_bit(5),   //!< コピー元許可
		CopyDest			= get_bit(6),   //!< コピー先許可
		IndirectArgument	= get_bit(7)	//!< 間接引数許可
	};
	//! @brief  バインド・フラグ・セット
	using BufferFlags = BitFlags<BufferFlag>;

	//! @brief	バッファリソースをどのリソースタイプとして参照するか
	//! @see	BindingType
	enum class BufferViewType {
		None,
		Buffer,
		RWBuffer,
		StructuredBuffer,
		RWStructuredBuffer,
		ByteAddressBuffer,
		RWByteAddressBuffer,
		ConstantBuffer,
	};

	struct BufferViewDesc {
		Ref<Buffer>		base; //!< ベースバッファ
		BufferViewType	type; //!< ビュータイプ
	};



	//! @brief  バッファ定義
	struct BufferDesc {

		String			name;			//!< 名前
		BufferState		state;			//!< バッファタイプ
		u64             size;			//!< バッファサイズ
		u32             stride;			//!< ストライド幅 StructuredBufferで使用する場合structureのサイズを設定する。
		BufferFlags     flags;			//!< バインドフラグ

	public:

		bool isValid() const;

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

		//! @brief  頂点バッファ用初期化
		template<typename TVertex>
		static BufferDesc Vertex(
			u64             count,
			BufferFlags     flags = BufferFlag::Vertex
		)
		{
			return BufferDesc(
				BufferState::Vertex,
				sizeof(TVertex)* count,
				sizeof(TVertex),
				flags
			);
		}

		//! @brief  インデックスバッファ用初期化
		template<typename TIndex>
		static BufferDesc Index(
			u64             count,
			BufferFlags     flags = BufferFlag::Index
		)
		{
			return BufferDesc(
				BufferState::Index,
				sizeof(TIndex) * count,
				sizeof(TIndex),
				flags
			);
		}

		//! @brief  定数バッファ用初期化
		//! @details サイズが256の倍数になるように調整されます。
		static BufferDesc Constant(
			u64				size,
			BufferState		initialState = BufferState::Constant,
			BufferFlags     flags = BufferFlag::Constant
		)
		{
			return BufferDesc(
				initialState,
				align_up(size, 256),
				0,
				flags
			);
		}

		//! @brief  ByteAddressバッファ用初期化
		//! @details サイズが256の倍数になるように調整されます。
		static BufferDesc ByteAddress(
			u64				size,
			BufferState		initialState = BufferState::ShaderResource,
			BufferFlags     flags = BufferFlag::ShaderResource
		)
		{
			return BufferDesc(
				initialState,
				size,
				0,
				flags
			);
		}

		//! @brief  ByteAddressバッファ用初期化
		//! @details サイズが256の倍数になるように調整されます。
		template<typename T>
		static BufferDesc Structured(
			u64				count,
			BufferState		initialState = BufferState::ShaderResource,
			BufferFlags     flags = BufferFlag::ShaderResource | BufferFlag::UnorderedAccess
		)
		{
			return BufferDesc(
				initialState,
				count * sizeof(T),
				sizeof(T),
				flags
			);
		}

	};

}
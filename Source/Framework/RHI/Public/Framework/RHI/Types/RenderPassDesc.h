//***********************************************************
//! @file
//! @brief		レンダーパス定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/TextureFormat.h>
#include <Framework/RHI/Types/ResourceState.h>

namespace ob::rhi {

#pragma region Enum

	//@―---------------------------------------------------------------------------
	//! @brief		アタッチメントのクリア方法
	//@―---------------------------------------------------------------------------
	enum class AttachmentClear {
		None,		//!< RenderPass開始時にクリアしない
		Clear,		//!< RenderPass開始時にTextureに指定された初期値でクリア
		DontCare,	//!< ハードウェアに任せる
	};
	
#pragma endregion

#pragma region Flag

	//@―---------------------------------------------------------------------------
	//! @brief		実行依存関係に関係するフラグ
	//@―---------------------------------------------------------------------------
	enum class PipelineStageFlag {
		None					= 0,			//!< 指定なし	
		TopOfPipe				= get_bit(0),	//!< 

		DrawIndirect			= get_bit(1),	//!< 
		VertexInput				= get_bit(2),	//!< 頂点バッファとインデックスバッファを消費するステージ
		VertexShader			= get_bit(3),	//!< 頂点シェーダステージ
		HullShader				= get_bit(4),	//!< ハルシェーダステージ
		DomainShader			= get_bit(5),	//!< ドメインシェーダステージ
		GeometryShader			= get_bit(6),	//!< ジオメトリシェーダステージ
		PixelShader				= get_bit(7),	//!< ピクセルシェーダステージ
		EarlyDepthStencilTest	= get_bit(8),	//!< 
		LateDepthStencilTest	= get_bit(9),	//!< 
		ColorAttachmentOutput	= get_bit(10),	//!< ピクセルシェーダの結果ブレンド後のステージ

		ComputeShader			= get_bit(11),	//!< コンピュートシェーダステージ

		CopyResolve				= get_bit(12),	//!< コピー、クリア、リゾルブ、クエリコピーなどのコマンド

		BottomOfPipe			= get_bit(13),	//!< 
		Host					= get_bit(14),	//!< 
		AllGraphics				= get_bit(15),	//!< 全てのグラフィック操作
		AllCommands				= get_bit(16),	//!< 全てのコマンド操作
	};
	using PipelineStageFlags = BitFlags<PipelineStageFlag>;


	//@―-----------------------------------------------------------------------------------------------------------
	//! @brief		メモリ依存関係に関係するメモリアクセスタイプを指定するフラグ
	//! @details	| OctbitEngine				| DirectX12						| Vulkan							|
	//!				|---------------------------|-------------------------------|-----------------------------------|
	//!				| *ResourceAccessFlag*		| *D3D12_RESOURCE_STATES*		| *VkAccessFlagBits*				|
	//!				| IndirectArgument			| INDIRECT_ARGUMENT				| INDIRECT_COMMAND_READ				|
	//!				| IndexBuffer				| INDEX_BUFFER					| INDEX_READ						|
	//!				| VertexBuffer				| VERTEX_AND_CONSTANT_BUFFER	| VERTEX_ATTRIBUTE_READ				|
	//!				| ConstantBuffer			| VERTEX_AND_CONSTANT_BUFFER	| UNIFORM_READ						|
	//!				| InputAttachment			| PIXEL_SHADER_RESOURCE			| INPUT_ATTACHMENT_READ				|		
	//!				| ShaderRead				| (NON_)PIXEL_SHADER_RESOURCE	| SHADER_READ						|
	//!				| ShaderWrite				| D3D12_UNORDERED_ACCESS		| SHADER_WRITE						|
	//!				| ColorRead					| RENDER_TARGET					| COLOR_ATTACHMENT_READ				|
	//!				| ColorWrite				| RENDER_TARGET					| COLOR_ATTACHMENT_WRITE			|
	//!				| DepthStencilRead			| DEPTH_READ					| DEPTH_STENCIL_ATTACHMENT_READ		|
	//!				| DepthStencilWrite			| DEPTH_WRITE					| DEPTH_STENCIL_ATTACHMENT_WRITE	|
	//!				| CopyRead					| COPY_SOURCE					| TRANSFER_READ						|
	//!				| CopyWrite					| COPY_DEST						| TRANSFER_WRITE					|
	//!				| AllRead					| GENERIC_READ					| MEMORY_READ						|
	//@―------------------------------------------------------------------------------------------------------------
	enum class ResourceAccessFlag {
		None				= 0,			//!< 指定なし
		IndirectArgument	= get_bit(0),	//!< DrawIndirectで使用するリソースの読み取りアクセス
		IndexBuffer			= get_bit(1),	//!< インデックスバッファの読み取りアクセス
		VertexBuffer		= get_bit(2),	//!< 頂点バッファの読み取りアクセス
		ConstantBuffer		= get_bit(3),	//!< 定数バッファの読み取りアクセス
		InputAttachment		= get_bit(4),	//!< 入力アタッチメントの読み取りアクセス
		ShaderRead			= get_bit(5),	//!< シェーダリソースの読み取り
		ShaderWrite			= get_bit(6),	//!< シェーダリソースの書き込み
		ColorRead			= get_bit(7),	//!< カラーアタッチメントの読み取り
		ColorWrite			= get_bit(8),	//!< カラーアタッチメントの書き込み
		DepthStencilRead	= get_bit(9),	//!< デプスステンシルアタッチメントの読み取り
		DepthStencilWrite	= get_bit(10),	//!< デプスステンシルアタッチメントの書き込み
		CopyRead			= get_bit(11),	//!< コピー元
		CopyWrite			= get_bit(12),	//!< コピー先
		AllRead				= get_bit(13),	//!< 全ての読み取り操作		移行先には指定しないでください
		AllWrite			= get_bit(14),	//!< 全ての書き込み操作

		// 未対応
		// StreamOutputWrite
		// StreamOutputCounterRead
		// StreamOutputCounterWrite
		// AccelerationStructureRead
		// AccelerationStructureWrite
		// ShadingRateAttachmentRead	VRS
		// Present
	};
	using ResourceAccessFlags = BitFlags<ResourceAccessFlag>;
	
#pragma endregion

	//@―---------------------------------------------------------------------------
	//! @brief		アタッチメント定義
	//! @details	* ステンシル操作はテクスチャフォーマットが深度ステンシルフォーマットの場合に使用されます。
	//@―---------------------------------------------------------------------------
	struct AttachmentDesc {
	public:
		TextureFormat					format;			//!< テクスチャフォーマット
		AttachmentClear					clear;			//!< レンダーパス開始時のクリア方式
		ResourceState					finalState;		//!< レンダーパス終了時のリソース状態
	};


	//@―---------------------------------------------------------------------------
	//! @brief		アタッチメント参照
	//@―---------------------------------------------------------------------------
	struct AttachmentReference {
		s32				index;	//!< RenderPassDesc::attachmentsのインデックス
		ResourceState	state;	//!< サブパスで使用するリソースの状態
	};


	//@―---------------------------------------------------------------------------
	//! @brief		サブパス定義
	//! @details	* 入力アタッチメントは他パスの出力を加工する場合に使用します
	//!				* 色アタッチメントは色情報の出力先です
	//!				* デプスステンシルアタッチメントはデプスの出力先です
	//!					* 処理の共通化のためArrayを使用していますが割り当て可能な最大数は1です
	//!				| 種類		| 説明								|
	//!				|-----------|-----------------------------------|
	//!				| inputs	| 他サブパスの結果読み取り			|
	//!				| colors	| 書き込み先						|
	//!				| depth		| デプス							|
	//@―---------------------------------------------------------------------------
	struct SubpassDesc {
		String							name;		//!< サブパス名
		Array<AttachmentReference>		inputs;		//!< 入力アタッチメントの参照
		Array<AttachmentReference>		colors;		//!< カラーアタッチメントの参照
		Array<AttachmentReference>		depth;		//!< デプスアタッチメントの参照
	};


	//@―---------------------------------------------------------------------------
	//! @brief		サブパスの依存グラフの辺
	//! @details	* stageMaskは通常ColorAttachmentOutputを使用します
	//!				* accessMaskは通常ColorWriteを使用します
	//@―---------------------------------------------------------------------------
	struct SubpassDependency {
		s32					srcSubpass;		//!< 依存元(-1の場合依存元なし)
		s32					dstSubpass;		//!< 依存先(-1の場合依存先なし)
		//PipelineStageFlags	srcStageMask;	//!< 依存元サブパスの全てのコマンドがどのステージまで進めばサブパスを開始できるか
		//PipelineStageFlags	dstStageMask;	//!< 全てのコマンドがどのステージまで進めば依存元サブパスを開始できるか
		//ResourceAccessFlag	srcAccessMask;	//!< メモリ依存性
		//ResourceAccessFlag	dstAccessMask;	//!< 
	};


	//@―---------------------------------------------------------------------------
	//! @brief		レンダーパス定義
	//! @details	事前にリソースの状態を指定することでGPU処理を最適化することができます。
	//!				DirectXでのCommandList:::ResourceBarrier()でリソースの状態を設定する代わりに
	//!				RenderPass開始時に事前に設定しておくことで効率的な描画が見込めます。
	//@―---------------------------------------------------------------------------
	struct RenderPassDesc {
		String							name;			//!< 名前
		Array<AttachmentDesc>			attachments;
		Array<SubpassDesc>				subpasses;
		Array<SubpassDependency>		dependencies;
	};



	//@―---------------------------------------------------------------------------
	//! @brief		AttachmentDesc の大小比較
	//@―---------------------------------------------------------------------------
	inline bool operator<(const ob::rhi::AttachmentDesc& a, const ob::rhi::AttachmentDesc& b) noexcept {
		if (a.format < b.format)return true;
		if (a.clear < b.clear)return true;
		if (a.finalState < b.finalState)return true;
		return false;
	}
	//@―---------------------------------------------------------------------------
	//! @brief		AttachmentReference の大小比較
	//@―---------------------------------------------------------------------------
	inline bool operator<(const ob::rhi::AttachmentReference& a, const ob::rhi::AttachmentReference& b) noexcept {
		if (a.index < b.index)return true;
		if (a.state < b.state)return true;
		return false;
	}
	//@―---------------------------------------------------------------------------
	//! @brief		SubpassDesc の大小比較
	//@―---------------------------------------------------------------------------
	inline bool operator<(const ob::rhi::SubpassDesc& a, const ob::rhi::SubpassDesc& b) noexcept {
		if (a.name < b.name)return true;
		if (a.inputs < b.inputs)return true;
		if (a.colors < b.colors)return true;
		if (a.depth < b.depth)return true;
		return false;
	}
	//@―---------------------------------------------------------------------------
	//! @brief		SubpassDependency の大小比較
	//@―---------------------------------------------------------------------------
	inline bool operator<(const ob::rhi::SubpassDependency& a, const ob::rhi::SubpassDependency& b) noexcept {
		if (a.srcSubpass < b.srcSubpass)return true;
		if (a.dstSubpass < b.dstSubpass)return true;
		return false;
	}
	//@―---------------------------------------------------------------------------
	//! @brief		RenderPassDesc の大小比較
	//@―---------------------------------------------------------------------------
	inline bool operator<(const ob::rhi::RenderPassDesc& a, const ob::rhi::RenderPassDesc& b) noexcept {
		return a.name < b.name;
		if (a.attachments < b.attachments)return true;
		if (a.subpasses < b.subpasses)return true;
		if (a.dependencies < b.dependencies)return true;
	}

}
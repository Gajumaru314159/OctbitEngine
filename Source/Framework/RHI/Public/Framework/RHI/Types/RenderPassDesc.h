//***********************************************************
//! @file
//! @brief		バッファ定義
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/TextureFormat.h>
#include <Framework/RHI/Types/ResourceState.h>

namespace ob::rhi {

#pragma region Enum

	//@―---------------------------------------------------------------------------
	//! @brief  アタッチメントのロード操作
	//@―---------------------------------------------------------------------------
	enum class AttachmentLoadOp {
		Load,		//!< そのまま使用
		Clear,		//!< 特定の値でクリア
		NotCare,	//!< ドライバやハードウェアに任せる
	};

	//@―---------------------------------------------------------------------------
	//! @brief  アタッチメントの保存操作
	//@―---------------------------------------------------------------------------
	enum class AttachmentStoreOp {
		Store,		//!< メモリに保存
		NotCare,	//!< ドライバやハードウェアに任せる
	};

	//@―---------------------------------------------------------------------------
	//! @brief  アタッチメントのサンプル数
	//@―---------------------------------------------------------------------------
	enum class MultiSampling {
		x01,		//!< マルチサンプリング x1
		x02,		//!< マルチサンプリング x2
		x04,		//!< マルチサンプリング x4
	};
	
#pragma endregion

#pragma region Flag

	//@―---------------------------------------------------------------------------
	//! @brief  アタッチメントのフラグ
	//@―---------------------------------------------------------------------------
	enum class AttachmentDescFlag {
		None		= get_bit(0),
		MayAlias	= get_bit(1),
	};
	using AttachmentDescFlags = BitFlags<AttachmentDescFlag>;


	//@―---------------------------------------------------------------------------
	//! @brief  パイプラインステージ・フラグ
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

	/*

	struct AttachmentReference {
		s32				index;	//!< RenderPassDesc::attachmentsのインデックス
		ResourceState	state;	//!< サブパスで使用するリソースの状態
	};

	//@―---------------------------------------------------------------------------
	//! @brief  サブパスの依存グラフの辺
	//@―---------------------------------------------------------------------------
	struct SubpassDependency {
		s32					srcSubpass;		//!< 依存元(-1の場合依存元なし)
		s32					dstSubpass;		//!< 依存先(-1の場合依存先なし)
		PipelineStageFlags	srcStageMask;	//!< 実行依存性
		PipelineStageFlags	dstStageMask;	//!< 
		ResourceAccessFlag	srcAccessMask;	//!< メモリ依存性
		ResourceAccessFlag	dstAccessMask;	//!< 
	};

	//@―---------------------------------------------------------------------------
	//! @brief		サブパス定義
	//! @details	* 入力アタッチメントは他パスの出力を加工する場合に使用します
	//!				* 色アタッチメントは色情報の出力先です
	//!				* リゾルブアタッチメントは出力をマルチサンプルして出力する先です
	//!					* 要素数は色アタッチメントと同じにしてください
	//!				* デプスステンシルアタッチメントはデプスの出力先です
	//!				* 保持アタッチメントは出力をメモリに書き戻さず保持する場合に使用します
	//!					* サブパスが2つ以下の場合使用しません
	//!					* サブパスで未使用のアタッチメントは未定義になるので保持マークを付けます
	//!				| 種類		| 説明								|
	//!				|-----------|-----------------------------------|
	//!				| inputs	| 他サブパスの結果読み取り			|
	//!				| colors	| 書き込み先						|
	//!				| resolve	| 書き込み先(マルチサンプル)		|
	//!				| depth		| デプス							|
	//!				| preserve	| 保持(上記で使用していないもの)	|
	//@―---------------------------------------------------------------------------
	struct SubpassDesc {
		Array<AttachmentReference>		inputs;		//!< 入力アタッチメントの参照
		Array<AttachmentReference>		colors;		//!< カラーアタッチメントの参照
		Array<AttachmentReference>		resolves;	//!< リゾル部アタッチメントの参照
		Optional<AttachmentReference>	depth;		//!< デプスアタッチメントの参照
		Array<AttachmentReference>		preserve;	//!< 保持アタッチメントの参照
	};

	//@―---------------------------------------------------------------------------
	//! @brief		アタッチメント定義
	//! @details	* ステンシル操作はテクスチャフォーマットが深度ステンシルフォーマットの場合に使用されます。
	//@―---------------------------------------------------------------------------
	struct AttachmentDesc {
	public:
		TextureFormat					format;			//!< テクスチャフォーマット
		AttachmentLoadOp				loadOp;			//!< レンダーパス開始時の操作
		AttachmentStoreOp				storeOp;		//!< レンダーパス終了時の操作
		AttachmentLoadOp				stencilLoadOp;	//!< レンダーパス開始時の操作(ステンシル)
		AttachmentStoreOp				stencilStoreOp;	//!< レンダーパス終了時の操作(ステンシル)
		ResourceState					initialState;	//!< レンダーパス開始時のリソース状態
		ResourceState					finalState;		//!< レンダーパス終了時のリソース状態
		MultiSampling					sampling;		//!< マルチサンプリング
		AttachmentDescFlags				flags;			//!< フラグ
		// TextureUsages usages;
	public:
		AttachmentDesc& setOp(AttachmentLoadOp load, AttachmentStoreOp store) {
			loadOp = load;
			storeOp = store;
			return *this;
		}
		AttachmentDesc& setStencilOp(AttachmentLoadOp load, AttachmentStoreOp store) {
			stencilLoadOp = load;
			stencilStoreOp = store;
			return *this;
		}
		AttachmentDesc& setState(ResourceState _initial, ResourceState _final) {
			initialState = _initial;
			finalState = _final;
			return *this;
		}
	};

	//@―---------------------------------------------------------------------------
	//! @brief		レンダーパス定義
	//! @details	事前にリソースの状態を指定することでGPU処理を最適化することができます。
	//!				DirectXでのCommandList:::ResourceBarrier()でリソースの状態を設定する代わりに
	//!				RenderPass開始時に事前に設定しておくことで効率的な描画が見込めます。
	//@―---------------------------------------------------------------------------
	struct RenderPassDesc {
		Array<AttachmentDesc>			attachments;
		Array<SubpassDesc>				subpasses;
		Array<SubpassDependency>		dependencies;
	};
	*/



	struct BeginEndOp {
		AttachmentLoadOp	load;		// clearValue
		AttachmentStoreOp	store;		// resolve params
	};

	//@―---------------------------------------------------------------------------
	//! @brief		カラーターゲットの操作
	//@―---------------------------------------------------------------------------
	struct RenderPassColorTargetDesc {
		TextureFormat	format;
		BeginEndOp		colorOp;
	};
	//@―---------------------------------------------------------------------------
	//! @brief		デプスターゲットの操作
	//@―---------------------------------------------------------------------------
	struct RenderPassDepthTargetDesc {
		TextureFormat	format;
		BeginEndOp		depthOp;
		BeginEndOp		stencilOp;
	};

	struct RenderPassDesc {
		Array<RenderPassColorTargetDesc> colors;
		Optional<RenderPassDepthTargetDesc> depth;
	};

}// namespcae ob::rhi
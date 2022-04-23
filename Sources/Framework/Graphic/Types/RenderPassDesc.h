//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Types/TextureFormat.h>
#include <Framework/Graphic/Types/ResourceState.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief  アタッチメントの追加のプロパティ
	//! @see    AttachmentDesc
	//@―---------------------------------------------------------------------------
	enum class AttachmentFlag :u32 {
		None		= get_bit(0),   //!< なし
		MayAlias	= get_bit(1),   //!< アタッチメントが他のアタッチメントと同じデバイスメモリをエイリアスすることを指定する。
	};
	using AttachmentFlags = bit_flags<AttachmentFlag>;


	//@―---------------------------------------------------------------------------
	//! @brief  サブパス開始時にアタッチメントをどう扱うか
	//! @see    AttachmentDesc
	//@―---------------------------------------------------------------------------
	enum class AttachmentLoadOp :u32 {
		Load,       //!< レンダーパス開始時点のデータを保持
		Clear,      //!< レンダーパス開始時に RenderPassBeginDesc で指定した値でクリア
		DontCare,   //!< デバイスに任せる(開始前の状態に依存しない場合に効果的)
	};


	//@―---------------------------------------------------------------------------
	//! @brief  サブパス終了時にアタッチメントをどう扱うか
	//! @see    AttachmentDesc
	//@―---------------------------------------------------------------------------
	enum class AttachmentStoreOp :u32 {
		Store,      //!< メモリに保存
		DontCare,   //!< デバイスに任せる
	};


	//@―---------------------------------------------------------------------------
	//! @brief  マルチサンプリング
	//@―---------------------------------------------------------------------------
	enum class MultiSampling:u32 {
		x01,
		x02,
		x04,
		x08,
		x016,
	};


	//@―---------------------------------------------------------------------------
	//! @brief  サブパスの使用方法を指定するフラグ
	//! @see    SubpassDesc
	//@―---------------------------------------------------------------------------
	enum class SubpassDescFlag :u32 {
		None = get_bit(0),   //!< なし
	};
	using AttachmentFlags = bit_flags<AttachmentFlag>;


	//@―---------------------------------------------------------------------------
	//! @brief  コマンドバッファへのパイプラインオブジェクトのバインドポイントを指定
	//! @see    SubpassDesc
	//@―---------------------------------------------------------------------------
	enum class PipelineBindPoint :u32 {
		Graphic = get_bit(0),   //!< グラフィックパイプラインとして指定
		Compute = get_bit(1),   //!< コンピュートパイプラインとして指定
		RayTracing = get_bit(2),   //!< レイトレーシングパイプラインとして指定
	};


	//@―---------------------------------------------------------------------------
	//! @brief		アタッチメント定義
	//@―---------------------------------------------------------------------------
	struct AttachmentDesc {
		AttachmentFlags		flags;			//!< フラグ
		TextureFormat		format;			//!< 書き込み先フォーマット
		u32					samplerCount;	//!< リソースのサンプル数
		AttachmentLoadOp	loadOp;			//!< レンダーパス開始時にデータをどうするか
		AttachmentStoreOp	storeOp;		//!< レンダーパス終了時にデータをどうするか
		ResourceState		initialState;	//!< 開始時リソースステート
		ResourceState		finalState;		//!< 終了時リソースステート
	};


	//@―---------------------------------------------------------------------------
	//! @brief		アタッチメント参照
	//! 
	//! @details	サブパス毎にどのアタッチメントを使用するかを定義します。
	//! @see		RenderPassDesc
	//@―---------------------------------------------------------------------------
	struct AttachmentReference {
		s32					attachment;		//!< RenderPassDesc の対応するインデックス(-1で無効)
		ResourceState		state;			//!< サブパス中にアタッチメントが使用するステート
	};


	//@―---------------------------------------------------------------------------
	//! @brief		サブパス定義
	//! 
	//! @details	アタッチメントをどのように処理するか定義する
	//@―---------------------------------------------------------------------------
	struct SubpassDesc {
		SubpassDescFlag				flags;						//!< フラグ
		PipelineBindPoint			pipelineBindPoint;			//!< バインドするパイプラインタイプ
		vector<AttachmentReference>	inputAttachments;			//!< 入力アタッチメント
		vector<AttachmentReference>	colorAttachments;			//!< カラーアタッチメント
		AttachmentReference			resolveAttachment;			//!< 解決アタッチメント(Nullable)
		AttachmentReference			depthStencilAttachment;		//!< デプスステンシルアタッチメント
		vector<u32>					preserveAttachments;		//!<
	};


	//@―---------------------------------------------------------------------------
	//! @brief		サブパス依存定義
	//@―---------------------------------------------------------------------------
	struct SubpassDependency {
		u32			srcSubpass;
		u32			dstSubpass;

	};

	//@―---------------------------------------------------------------------------
	//! @brief		レンダーパス定義
	//@―---------------------------------------------------------------------------
	struct RenderPassDesc {
		vector<AttachmentDesc>		attachments;
		vector<SubpassDesc>			subpasses;
		vector<SubpassDependency>	dependencies;
	};

}// namespcae ob
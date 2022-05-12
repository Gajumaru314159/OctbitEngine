//***********************************************************
//! @file
//! @brief		バッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/Utility/GraphicObjectHolder.h>
#include <Framework/Graphic/Types/BufferDesc.h>
#include <Framework/Core/Misc/Blob.h>

namespace ob::graphic {

	//! @cond
	enum class MapMode {
		Read,
		Write,
	};

	struct UpdateResourceParameter {
		void* pData;
		size_t offset;
	};
	//! @endcond


	//@―---------------------------------------------------------------------------
	//! @brief      バッファ
	//@―---------------------------------------------------------------------------
	class Buffer {
		OB_DEFINE_GRAPHIC_OBJECT_HOLDER(Buffer);
	public:

		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//@―---------------------------------------------------------------------------
		Buffer();


		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//! 
		//! @param desc バッファ定義
		//! @param name 名前
		//@―---------------------------------------------------------------------------
		Buffer(const BufferDesc& desc, StringView name = TC("Buffer"));


		//@―---------------------------------------------------------------------------
		//! @brief  コンストラクタ
		//! 
		//! @param desc バッファ定義
		//! @param blob 初期化データ
		//! @param name 名前
		//@―---------------------------------------------------------------------------
		Buffer(const BufferDesc& desc, const Blob& blob, StringView name = TC("Buffer"));


		//@―---------------------------------------------------------------------------
		//! @brief  定義を取得
		//@―---------------------------------------------------------------------------
		const BufferDesc& getDesc()const;


		//@―---------------------------------------------------------------------------
		//! @brief  バッファサイズを取得
		//@―---------------------------------------------------------------------------
		size_t size()const;


		//@―---------------------------------------------------------------------------
		//! @brief      バッファを更新
		//! 
		//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
		//@―---------------------------------------------------------------------------
		void update(size_t size, const void* pData, size_t offset = 0);


		//@―---------------------------------------------------------------------------
		//! @brief      バッファを更新(直接更新)
		//! 
		//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
		//@―---------------------------------------------------------------------------
		void updateDirect(size_t size, const void* pData, size_t offset = 0);


	private:

		void beginUpdate(UpdateResourceParameter&);
		void endUpdate(UpdateResourceParameter&);

	};


}// namespace pb::graphic
//***********************************************************
//! @file
//! @brief		バッファ
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Misc/Blob.h>
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/BufferDesc.h>
#include <Framework/RHI/Types/BindlessHandle.h>

namespace ob::rhi {

	//! @brief      バッファ
	class Buffer :public GraphicObject {
	public:
		using CopyFunc = Func<void(void*)>;
	public:

		//! @brief  空のバッファを作成
		static Ref<Buffer> Create(const BufferDesc& desc);

		//! @brief  初期データを指定してバッファを作成
		static Ref<Buffer> Create(const BufferDesc& desc, BlobView blob);

		//! @brief  ベースのバッファを指定して異なるビューを持つバッファを作成
		static Ref<Buffer> Create(const BufferViewDesc& desc);

	public:

		//! @brief		定義を取得
		virtual const BufferDesc& getDesc()const = 0;

		//! @brief      BindlessHandleを取得
		virtual BindlessHandle getHandle()const = 0;

		//! @brief      バッファを更新
		//! @note		deprecated
		//! 
		//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
		virtual void update(size_t size, const void* pData, size_t offset = 0) = 0;

		//! @brief      バッファを更新(直接更新)
		//! 
		//! @details    map / unmap と異なり、バッファの更新は描画スレッドの直前にまとめて行われます。
		virtual void update(const CopyFunc& func) = 0;

		//! @brief      バッファを更新(直接更新)
		//! @note		deprecated
		virtual void updateDirect(size_t size, const void* pData, size_t offset = 0) = 0;

		//! @brief      バッファを更新(直接更新)
		virtual void updateDirect(const CopyFunc& func) = 0;

		//! @brief      バッファを更新(直接更新)
		template<class T>
		void updateDirect(const T& value, size_t offset) {
			updateDirect(sizeof(T), &value, offset);
		}

	};

}
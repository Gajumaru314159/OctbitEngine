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

    struct UpdateResourceParameter {
        void* pData;
        size_t offset;
    };


    //@―---------------------------------------------------------------------------
    //! @brief      バッファ
    //@―---------------------------------------------------------------------------
    class Buffer{
        friend class Device;
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
        //@―---------------------------------------------------------------------------
        Buffer(const BufferDesc& desc);


        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //! 
        //! @param desc バッファ定義
        //! @param data 初期化データ
        //@―---------------------------------------------------------------------------
        Buffer(const BufferDesc& desc, const Blob& data);


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
        template<typename F>
        void update(size_t offset, size_t size, F&& func);

    private:

        void beginUpdate(UpdateResourceParameter&);
        void endUpdate(UpdateResourceParameter&);

    };


}// namespace pb::graphic






//===============================================================
// インライン
//===============================================================
//! @cond
namespace ob::graphic {

    //@―---------------------------------------------------------------------------
    //! @brief  バッファを更新
    //@―---------------------------------------------------------------------------
    template<typename F>
    void Buffer::update(size_t offset, size_t size, F&& func) {
        UpdateResourceParameter param{};
        beginUpdate(param);
        func(param.pData);
        endUpdate(param);
    }

}
//! @endcond
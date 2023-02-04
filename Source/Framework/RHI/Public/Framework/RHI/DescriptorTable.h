//***********************************************************
//! @file
//! @brief		デスクリプタ・テーブル・インターフェイス
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/GraphicObject.h>
#include <Framework/RHI/Types/DescriptorDesc.h>

//===============================================================
// クラス宣言
//===============================================================
namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief      デスクリプタ・テーブル
    //@―---------------------------------------------------------------------------
    class DescriptorTable :public GraphicObject {
    public:

        //@―---------------------------------------------------------------------------
        //! @brief              コンストラクタ
        //!
        //! @param type         デスクリプタに設定するリソースの種類
        //! @param elementNum   要素数
        //! @param name         デバッグ名
        //@―---------------------------------------------------------------------------
        static Ref<DescriptorTable> Create(DescriptorHeapType type, s32 elementNum);

    public:

        //@―---------------------------------------------------------------------------
        //! @brief  リソースを設定
        //@―---------------------------------------------------------------------------
        //! @{
        virtual bool setResource(s32 index, const Ref<class Buffer>& resource) = 0;
        virtual bool setResource(s32 index, const Ref<class Texture>& resource) = 0;
        //virtual bool setResource(s32 index, class Sampler& resource) = 0;
        //! @}

    protected:

        virtual ~DescriptorTable() = default;

    };

}// namespace pb::rhi
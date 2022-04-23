//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob {

    enum class ViewType :u32 {
        ConstantBuffer,
        ShaderResource,
        UnorderedAccess,
        RenderTarget,
        DepthStencil,
        VertexBuffer,
        IndexBuffer,
        StreamOutputBuffer,
        Sampler,
    };

    enum class ResourceFormat {

    };

    enum class ViewDimension {
        Texture1D,
        Texture1DArray,
        Texture2D,
        Texture2DArray,
        Texture3D,
        TextureCube,
        TextureCubeArray,
        ConstantBuffer,
        BufferTyped,
        BufferStructured,
        BufferByteAddress,
        AccelerationStructure,
        VertexBuffer,
        IndexBuffer,
        StreamOutput,
        Sampler
    };

    struct ViewDesc {
        ViewType type;
        ResourceFormat format;
        ViewDimension dimension;
    };

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class IView {
    public:
        virtual const ViewDesc getViewDesc()const = 0;
        virtual IResource* getResource()const = 0;
        //virtual const 
    };

    class IVertexBufferView :public IView {
    public:
        virtual const VertexBufferViewDesc getDesc()const = 0;
    };
    class IVertexBufferView :public IView {
    public:
        virtual const VertexBufferViewDesc getDesc()const = 0;
    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}// namespcae ob
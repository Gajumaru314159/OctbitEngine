//***********************************************************
//! @file
//! @brief		前方宣言
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphic {

    class INameable;
    class IBlob;

    class IShader;

    class IDeviceFactory;
    class IDebugMessage;
    class IDebugMessageQueue;
    class IDeviceAdapter;
    class ISurface;
    class IDevice;
    class IDeviceChild;
    class ISwapChain;

    class IResourceHeap;
    class IResource;
    class IBuffer;
    class ITexture;

    class IView;
    class IVertexBufferView;
    class IIndexBufferView;
    class IConsttantBufferView;
    class IRenderTargetView;
    class IDepthStencilView;
    class ShaderResourceView;
    class IUnordererdAccessView;
    class ISamplerView;
    class IStreamOutputBufferView;

    class IFramebuffer;
    class IRenderPass;
    class IDescriptorPool;
    class IDescriptorSet;
    class IRootSignature;
    class IPipelineState;

    class IDescriptorHeap;
    class IDescriptorPool;
    class IDescriptorSet;
    class IDescriptorUpdate;
    class IDescriptorSetLayout;
    class IPipelineLayout;

    class ICommandQueue;
    class ICommandAllocator;
    class ICommandList;
    class IFence;

    class IQueryHeap;
    class ICommandSignature;

    class IAccelerationStructure;

}// namespcae ob::graphic
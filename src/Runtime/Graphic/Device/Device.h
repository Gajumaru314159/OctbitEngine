﻿//***********************************************************
//! @file
//! @brief		グラフィックAPI
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Foundation.hpp>

namespace ob {
    namespace graphic {

        class CommandList;



        class Device {
        public:

            void BeginRender();
            void EntryDirecCommandQueue(CommandList&);
            void EndRender();
            void Flip();

            void CreateCommandList(ICommandList* list, CommandListType);// Graphic/Compute
            void CreateComputeCommandQueue(ComputeCommandQueue,s32 listCapacity);

            void CreateDepthSurface();
            void CreateTargetTexture(texture, desc, pName);
            void CreateResourceTexture(texture, desc,BinFlag, pName);
            void CreateEmptyTexture(texture,desc,pName);

            void CreateBuffer();
            void CreateDynamicConstantBuffer();
            void CreateFrameBuffer();

            void CreateRenderPass();
            void CreateRootSignature();
            void CreateCommandSignature();

            void CreateVertexShader();
            // P,G,H,D,Compute,Raygen,Closest,AnyHit,Miss

            void CreatePipelineState();
            void CrateComputtePipelineState();
            void CreateSamplerStatt();
            void CreateRootDescroptorTable();
            void CreateRootDescriptor();
            void CreateDepthStencilView();
            void CreateRenderTargetView();

            void CreateShaderResourceView();
            void CreateUnordererAccessView();

            void CreateOcclusionQuery();
            void CreateDynamicPrimittiveBuffer();


            void GetSystemCommandList();
            void GetMainDisplay();
            void ChangeSyncType();

            bool IsOutputHDRSupported();
            bool IsMultiDrawSupported();
            bool IsAsyncComputeCompute();

            // define SUB_DISPLAY
            void CreateSubDisplay();

#if defined(OS_WINDOWS)

            void CreateVideoCradInfoList();

            void ChangeVideoOutputDesc();
            void ChangeMainDisplayDesc();

            void AddRestoreEvent();

            void GetNowVideoOutputDesc();
            void GetNewVideoOutputDesc();

            bool IsVideoOutputDescChanged();

            bool CanUseMultiSampling();

#endif

            /*
UE4
Create:
    samplerState
    RasterizerSate
    DepthStencilState
    BlendState

    VertexDeclaration

    Shader  VPHDGC

    RenrerQueryPool
    ComputeFence

    StagingBuffer
    BoundShaderState
    GraphicsPipelineState
    ComputingPipelineState
    UniformBuffer
    IndexBuffer
    VertexBuffer
    StructuredBuffer


    UnordererdAccessView
    ShaderResourceView

    Texture2D
    AsyncCreateTexture2D

    ShaderResourceView
    ShaderResourceViewWriteMask
    ShaderResourceViewFMask
end


    LockStagingBuffer
    UnllockStagingBuffer        
            
            */

        };


    }// namespace graphic
}// namespace ob


/*

Adapter
Allocation
Buffer
CommandContext
CommandList
    Commands
ConstantBuffer
DesctriptorCatch
Device
DirectCommandListManager
IndexBuffer
PipelineState
Query
RayTracing
RenderTarget
Residency
RootSignature
Shader
State
StateCache
Stats
StructuredBuffetr
Txture
UAV
UnifotrmBuffer
VertexBuffer
VertexDeclaration
View
Viewport


*/
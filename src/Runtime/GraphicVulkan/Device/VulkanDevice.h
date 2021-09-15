//***********************************************************
//! @file
//! @brief		グラフィックAPI
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Foundation/Foundation.hpp>

namespace ob {
    namespace graphic {

        class IDevice {
        public:

            void BeginRender();
            void EntryDirecCommandQueue(ICommandList&);
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

        };


    }// namespace graphic
}// namespace ob
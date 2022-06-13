//***********************************************************
//! @file
//! @brief		コマンドキュー
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/CommandList.h>

namespace ob::graphic::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  コマンドキュー
    //@―---------------------------------------------------------------------------
    class CommandQueue {
    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  コンストラクタ
        //@―---------------------------------------------------------------------------
        CommandQueue(class DeviceImpl& device);

        void entryCommandList(const CommandList& commandList);

        void execute();
        void wait();

        ComPtr<ID3D12CommandQueue>& getNative() {
            return m_commandQueue;
        }

        void setName(StringView name);

    private:

        ComPtr<ID3D12CommandQueue> m_commandQueue;
        ComPtr<ID3D12Fence> m_fence;
        UINT64 m_fenceVal;
        HANDLE m_fenceEvent;

        Array<ID3D12CommandList*> m_entriedNativeCommandList;
        Array<CommandList> m_entriedCommandList;

    };

}// ob::graphic::dx12 
﻿//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/RHI/Types/RenderPassDesc.h>
#include <Framework/RHI/Types/TextureFormat.h>

namespace ob::rhi {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    struct RenderPassDescHelper:RenderPassDesc {
    public:

        enum class AttachmentID :s32 {};

    public:

        //===============================================================
        // コンストラクタ / デストラクタ
        //===============================================================

        //@―---------------------------------------------------------------------------
        //! @brief  アタッチメント追加
        //@―---------------------------------------------------------------------------
        AttachmentID addAttachment(TextureFormat format, AttachmentClear clear=AttachmentClear::Clear, ResourceState finalState = ResourceState::PixelShadeResource) {
            auto& attachment = attachments.emplace_back();
            attachment.format = format;
            attachment.clear = clear;
            attachment.finalState = finalState;
            return static_cast<AttachmentID>(attachments.size() - 1);
        }

        //@―---------------------------------------------------------------------------
        //! @brief  サブパス追加
        //@―---------------------------------------------------------------------------
        s32 addSubpass(const Array<AttachmentID>& inputs, const Array<AttachmentID>& colors, Optional<AttachmentID> depth) {

            // サブパス設定
            auto& subpass = subpasses.emplace_back();

            for (auto& inputID : inputs) {
                auto& ref = subpass.inputs.emplace_back();
                ref.index = static_cast<s32>(inputID);
                ref.state = ResourceState::PixelShadeResource;
            }
            for (auto& colorID : colors) {
                auto& ref = subpass.colors.emplace_back();
                ref.index = static_cast<s32>(colorID);
                ref.state = ResourceState::ColorAttachment;
            }
            if(depth.has_value()) {
                auto& ref = subpass.depth.emplace_back();
                ref.index = static_cast<s32>(depth.value());

                auto format = attachments.at(ref.index).format;
                if (TextureFormatUtility::HasStencil(format)) {
                    ref.state = ResourceState::DepthStencilAttachment;
                } else {
                    ref.state = ResourceState::DepthAttachment;
                }

            }

            return static_cast<s32>(subpasses.size()-1);
        }

        //@―---------------------------------------------------------------------------
        //! @brief  サブパス追加
        //@―---------------------------------------------------------------------------
        s32 addSubpassICD(const Array<AttachmentID>& inputs, const Array<AttachmentID>& colors, AttachmentID depth ) {
            return addSubpass(inputs, colors, depth);
        }
        //@―---------------------------------------------------------------------------
        //! @brief  サブパス追加
        //@―---------------------------------------------------------------------------
        s32 addSubpassXCD(const Array<AttachmentID>& colors, AttachmentID depth) {
            return addSubpass({}, colors, depth);
        }
        //@―---------------------------------------------------------------------------
        //! @brief  サブパス追加
        //@―---------------------------------------------------------------------------
        s32 addSubpassXXD(AttachmentID depth) {
            return addSubpass({}, {}, depth);
        }
        //@―---------------------------------------------------------------------------
        //! @brief  サブパス追加
        //@―---------------------------------------------------------------------------
        s32 addSubpassXCX(const Array<AttachmentID>& colors) {
            return addSubpass({}, colors, std::nullopt);
        }
        //@―---------------------------------------------------------------------------
        //! @brief  サブパス追加
        //@―---------------------------------------------------------------------------
        s32 addSubpassICX(const Array<AttachmentID>& inputs, const Array<AttachmentID>& colors) {
            return addSubpass(inputs, colors, std::nullopt);
        }

        //@―---------------------------------------------------------------------------
        //! @brief  サブパス追加
        //@―---------------------------------------------------------------------------
        void addDependeny(s32 src, s32 dst) {

            auto& dependency = dependencies.emplace_back();

            dependency.srcSubpass = src;
            dependency.dstSubpass = dst;

        }

    };

}// namespcae ob
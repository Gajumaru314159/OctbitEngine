//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Engine/Name.h>

namespace ob::graphics {

    using Name = Name;

    enum class SlotType {
        Input,
        Output,
        InputOutput,
    };


    enum class ScopeAttachmentUsage
    {
        RenderTarget,
        DepthStencil,
        Shader,
        Copy,
        Resolve,
        Predication,
        Indirect,
        SubpassInput,
        InputAssembly,
    };
    enum class AttachmentLifetimeType : uint32_t
    {
        Imported,   //
        Transient,
    };


    struct Slot {
        Name name;      // Albedo
        String shaderInputName; // m_mainTex
        SlotType slotType;
        // usage
        LoadStore loadStore;
        Format format;
        Dimension;
    };

    struct PassRequest {
        Name name;

    };


    struct PassAttachmentSizeSource {
        // ref
        Vec3 multiplier;
    };

    struct ImageAttachment{
        Name name;
        f32 lifetime;
        PassAttachmentSizeSource sizeSource;

        // FormatSource
        // ImageDesc
    };


    struct PassSlot {
        Name        name;               // DownsampleSource
        Name        shaderInputName;    // m_inputTexture
        SlotType    slotType;           // Input
        ScopeAttachmentUsage scopeAttachmentUsage;// Shader
        // LoadStore
        // FormatFilters
        // DimensionFilters
    };

    struct PassAttachmentDesc {
        Name        name;   // DownSampledResult
        AttachmentLifetimeType lifetime;

    };
    
    struct PassConnection {

    };

    struct PassFallbackConnection {

    };

    struct PassTemplate {
        Name name;      // DownsamplePassTemplate
        Name passClass; // Computepass
        Array<PassSlot> slots;
        Array<ImageAttachment> attachments;
    };

}
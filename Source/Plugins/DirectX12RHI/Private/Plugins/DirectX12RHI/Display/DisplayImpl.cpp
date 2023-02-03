//***********************************************************
//! @file
//! @brief		ディスプレイ・チェーン実装(DirectX12)
//! @author		Gajumaru
//***********************************************************
#include "DisplayImpl.h"
#include <Framework/RHI/RenderPass.h>
#include <Framework/RHI/FrameBuffer.h>
#include <Plugins/DirectX12RHI/Device/DeviceImpl.h>
#include <Plugins/DirectX12RHI/Texture/TextureImpl.h>
#include <Plugins/DirectX12RHI/Command/CommandListImpl.h>
#include <Plugins/DirectX12RHI/Utility/Utility.h>
#include <Plugins/DirectX12RHI/Utility/TypeConverter.h>

namespace {
    int static const s_maxDisplayCount = 4;
}

namespace ob::rhi::dx12 {

    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    DisplayImpl::DisplayImpl(DeviceImpl& rDevice, const DisplayDesc& desc)
        : m_desc(desc)
    {

        if (!desc.window.isValid()) {
            LOG_ERROR_EX("Graphic","Windowが設定されていません。");
            return;
        }

        // サイズが指定されていない場合はウィンドウサイズを使用
        if (m_desc.size.width == 0 || m_desc.size.height == 0)m_desc.size = m_desc.window.getSize();

        m_syncInterval = desc.vsync ? 1 : 0;

        if (!createDisplay(rDevice))return;
        if (!createResources(rDevice))return;
        if (!createBuffers(rDevice))return;

        m_initialized = true;

    }


    //@―---------------------------------------------------------------------------
    //! @brief      名前を取得
    //@―---------------------------------------------------------------------------
    const String& DisplayImpl::getName()const {
        return m_desc.name;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  スワップチェーン生成
    //@―---------------------------------------------------------------------------
    bool DisplayImpl::createDisplay(DeviceImpl& rDevice) {
        auto& window = m_desc.window;

        UINT sampleQuarity = 0;
        UINT sampleCount = 1;
        HWND hWnd = (HWND)window.getHandle();
        {
            D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS feature{};
            auto result = rDevice.getNative()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &feature, sizeof(feature));
            if (SUCCEEDED(result)) {
                LOG_INFO_EX("Graphic", "最大マルチサンプルカウント={}", feature.SampleCount);
                LOG_INFO_EX("Graphic", "最大マルチサンプルクオリティ={}", feature.NumQualityLevels);
                //sampleQuarity = feature.SampleCount;
                //sampleCount = feature.NumQualityLevels;
            }
        }

        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferDesc.Width = m_desc.size.width;                                 // 画面解像度【横】
        swapChainDesc.BufferDesc.Height = m_desc.size.height;                               // 画面解像度【縦】
        swapChainDesc.BufferDesc.Format = TypeConverter::Convert(m_desc.format);            // ピクセルフォーマット
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60000;                             // リフレッシュ・レート分子
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1000;                            // リフレッシュ・レート分母
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;   // スキャンラインの順番 => 指定なし
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;                     //解像度に合うように同補正するか => 拡大

        swapChainDesc.SampleDesc.Quality = sampleQuarity;                                   // マルチサンプル・クオリティ
        swapChainDesc.SampleDesc.Count = sampleCount;                                       // マルチサンプル・カウント

        swapChainDesc.BufferCount = m_desc.bufferCount;						                // バッファの数
        swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;						            // バックバッファとして使用
        swapChainDesc.OutputWindow = hWnd;                                                  // ウィンドウ
        swapChainDesc.Windowed = TRUE;                                                      // ※公式リファレンスによるとフルスクリーン指定は別ので行う
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;                           // Present後破棄

        swapChainDesc.Flags =
            //DXGI_SWAP_CHAIN_FLAG_NONPREROTATED |                  // フルスクリーン時自動回転
            DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH |                // ResizeTargetでサイズ変更許可
            //DXGI_SWAP_CHAIN_FLAG_DISPLAY_ONLY |                   // リモートアクセス禁止
            //DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT |  // フルスクリーン以外で描画待機
            //DXGI_SWAP_CHAIN_FLAG_FULLSCREEN_VIDEO |               // フルスクリーンビデオ
            //DXGI_SWAP_CHAIN_FLAG_YUV_VIDEO |                      // YUVビデオのスワップチェーン
            0;

        // スワップチェイン生成
        auto result = rDevice.getFactory()->CreateSwapChain(
            rDevice.getCommandQueue().Get(),
            &swapChainDesc,
            (IDXGISwapChain**)m_swapChain.ReleaseAndGetAddressOf());

        if (FAILED(result)) {
            Utility::OutputFatalLog(result, TC("IDXGIFactory::CreateSwapChain()"));
            return false;
        }


        if (window.isMainWindow()) {

            // Alt + Enter でウィンドウモードに変わらないようにする 
            rDevice.getFactory()->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

            if (window.getMode() == platform::WindowMode::FullScreen) {
                // TODO フルスクリーンの場合バックバッファをリサイズ
            }
        }

        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      レンダーテクスチャを初期化
    //@―---------------------------------------------------------------------------
    bool DisplayImpl::createBuffers(DeviceImpl& rDevice) {

        if (!is_in_range(m_desc.bufferCount, 1, s_maxDisplayCount)) {
            LOG_ERROR_EX("Graphic", "バックバッファの枚数が不正です。[Min=1,Max={0},Value={1}]", s_maxDisplayCount, m_desc.bufferCount);
            return false;
        }

        HRESULT result;

        // バッファを生成
        m_textures.reserve(m_desc.bufferCount);
        m_buffers.reserve(m_desc.bufferCount);

        // レンダーターゲットビュー生成
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = TypeConverter::Convert(m_desc.format);
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

        for (s32 i = 0; i < m_desc.bufferCount; ++i) {

            ComPtr<ID3D12Resource> resource;
            result = m_swapChain->GetBuffer(i, IID_PPV_ARGS(resource.ReleaseAndGetAddressOf()));
            if (FAILED(result)) {
                // 生成が正しければ呼ばれないはず
                Utility::OutputFatalLog(result, TC("IDXGIDisplay::GetBuffer()"));
                return false;
            }

            auto& texture = m_textures.emplace_back(new TextureImpl(rDevice,resource,D3D12_RESOURCE_STATE_PRESENT));

            {
                FrameBufferDesc bdesc;
                bdesc.renderPass = m_renderPass;
                bdesc.attachments.push_back(texture);

                auto& buffer = m_buffers.emplace_back(FrameBuffer::Create(bdesc));
                OB_ASSERT_EXPR(buffer);
            }

            m_viewport = CD3DX12_VIEWPORT(resource.Get());
            m_scissorRect = CD3DX12_RECT(0, 0, (UINT)m_viewport.Width, (UINT)m_viewport.Height);
        }

        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  コンストラクタ
    //@―---------------------------------------------------------------------------
    bool DisplayImpl::createResources(DeviceImpl& rDevice) {
        // レンダーパス
        {
            RenderPassDescHelper rdesc;
            auto color = rdesc.addAttachment(m_desc.format);
            auto pass0 = rdesc.addSubpassXCX({ color });

            m_renderPass = RenderPass::Create(rdesc);
            OB_ASSERT_EXPR(m_renderPass);
        }

        {
            Vec2 vertices[] = {
                {-1,-1},
                {+1,-1},
                {-1,+1},
                {+1,-1},
                {+1,+1},
                {-1,+1},
            };
            BufferDesc bdesc = BufferDesc::Vertex<Vec2>(std::size(vertices));
            bdesc.name = m_desc.name + TC("_Vertices");
            m_verices = Buffer::Create(bdesc);
            m_verices->updateDirect(bdesc.bufferSize, vertices);
        }

        Ref<Shader> vs;
        Ref<Shader> ps;
        {
            String code;
            code.append(TC("SamplerState g_mainSampler:register(s0);						\n"));
            code.append(TC("Texture2D g_mainTex:register(t0);								\n"));
            code.append(TC("// IN / OUT														\n"));
            code.append(TC("struct VsIn {													\n"));
            code.append(TC("  float2 pos	:POSITION;										\n"));
            code.append(TC("};																\n"));
            code.append(TC("struct PsIn {													\n"));
            code.append(TC("  float4 pos	:SV_POSITION;									\n"));
            code.append(TC("  float2 uv	    :TEXCOORD;									    \n"));
            code.append(TC("};																\n"));
            code.append(TC("// エントリ														\n"));
            code.append(TC("PsIn VS_Main(VsIn i) {											\n"));
            code.append(TC("    PsIn o;														\n"));
            code.append(TC("    o.pos = float4(i.pos*float2(2,-2)-1,0,1);				    \n"));
            code.append(TC("    o.uv = i.pos.xy;								            \n"));
            code.append(TC("    return o;													\n"));
            code.append(TC("}																\n"));
            code.append(TC("float4 PS_Main(PsIn i):SV_TARGET0{								\n"));
            code.append(TC("    return g_mainTex.Sample(g_mainSampler,i.uv);		        \n"));
            code.append(TC("}																\n"));

            vs = VertexShader::Create(code);
            ps = PixelShader::Create(code);
            OB_ASSERT_EXPR(vs && ps);
        }

        Ref<RootSignature> signature;
        {
            RootSignatureDesc desc(
                {
                    RootParameter::Range(DescriptorRangeType::SRV,1,0),
                },
                {
                    StaticSamplerDesc(SamplerDesc(),0),
                }
                );
            desc.name = m_desc.name;
            signature = RootSignature::Create(desc);
            OB_ASSERT_EXPR(signature);
        }

        Ref<PipelineState> pipeline;
        {
            PipelineStateDesc desc;
            desc.name = m_desc.name;
            desc.renderPass = m_renderPass;
            desc.subpass = 0;

            desc.rootSignature = signature;
            desc.vs = vs;
            desc.ps = ps;
            desc.vertexLayout.attributes = {
                VertexAttribute(Semantic::Position,0,Type::Float,2),
            };
            desc.blend[0] = BlendDesc::AlphaBlend;
            desc.rasterizer.cullMode = CullMode::None;
            desc.depthStencil.depth.enable = false;
            desc.depthStencil.stencil.enable = false;

            pipeline = PipelineState::Create(desc);
            OB_ASSERT_EXPR(pipeline);
        }

        m_signature = signature;
        m_pipeline = pipeline;

        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      カラースペースを設定
    //@―---------------------------------------------------------------------------
    bool DisplayImpl::setColorSpace() {
        bool isHdrEnabled = m_desc.hdr;
        if (!isHdrEnabled)return false;

        // TODO Rec2020以外の指定対応
        DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
        UINT colorSpaceSupport;

        auto result = m_swapChain->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport);
        if (FAILED(result)) {
            Utility::OutputFatalLog(result, TC("IDXGIDisplay::CheckColorSpaceSupport()"));
            return false;
        }

        if (colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT) {
            result = m_swapChain->SetColorSpace1(colorSpace);
            if (FAILED(result)) {
                Utility::OutputFatalLog(result, TC("IDXGIDisplay::SetColorSpace1()"));
                return false;
            }
        }
        return true;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  デストラクタ
    //@―---------------------------------------------------------------------------
    DisplayImpl::~DisplayImpl() {

    }


    //@―---------------------------------------------------------------------------
    //! @brief      表示するテクスチャをバインド
    //@―---------------------------------------------------------------------------
    void DisplayImpl::bindTexture(const Ref<Texture> texture) {

        if (m_bindedTexture == texture)
            return;

        m_bindedTextureTable.reset();
        m_bindedTexture = texture;
        
        if (m_bindedTexture) {
            m_bindedTextureTable = DescriptorTable::Create(DescriptorHeapType::CBV_SRV_UAV,1);
            m_bindedTextureTable->setResource(0,m_bindedTexture);
        }

    }


    //@―---------------------------------------------------------------------------
    //! @brief  妥当なオブジェクトか
    //@―---------------------------------------------------------------------------
    bool DisplayImpl::isValid()const {
        return m_initialized;
    }


    //@―---------------------------------------------------------------------------
    //! @brief  定義を取得
    //@―---------------------------------------------------------------------------
    const DisplayDesc& DisplayImpl::getDesc()const noexcept {
        return m_desc;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      更新
    //! 
    //! @details    表示するテクスチャを次のバックバッファにします。
    //@―---------------------------------------------------------------------------
    void DisplayImpl::update() {
        
        if (!m_desc.window.isValid())return;

        auto result = m_swapChain->Present(m_syncInterval, 0);

        if (FAILED(result)) {
            Utility::OutputFatalLog(result, TC("IDXGUIDisplay::Present()"));
            LOG_FATAL_EX("Graphic","スワップチェーンの更新に失敗")
            return;
        }

        auto index = m_swapChain->GetCurrentBackBufferIndex();
        m_textures.setIndex(index);
        m_buffers.setIndex(index);

    }


    //@―---------------------------------------------------------------------------
    //! @brief      デスクリプタCPUハンドルを取得
    //@―---------------------------------------------------------------------------
    D3D12_CPU_DESCRIPTOR_HANDLE DisplayImpl::getCpuHandle()const {
        return m_textures.current().cast<TextureImpl>()->getRTV().getCpuHandle();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      デスクリプタGPUハンドルを取得
    //@―---------------------------------------------------------------------------
    D3D12_GPU_DESCRIPTOR_HANDLE DisplayImpl::getGpuHandle()const {
        return m_textures.current().cast<TextureImpl>()->getRTV().getGpuHandle();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      ビューポートを取得
    //@―---------------------------------------------------------------------------
    D3D12_VIEWPORT DisplayImpl::getViewport()const {
        return m_viewport;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      シザー矩形を取得
    //@―---------------------------------------------------------------------------
    D3D12_RECT DisplayImpl::getScissorRect()const {
        return m_scissorRect;
    }


    //@―---------------------------------------------------------------------------
    //! @brief      リソース取得
    //@―---------------------------------------------------------------------------
    ID3D12Resource* DisplayImpl::getResource()const {

        return m_textures.current().cast<TextureImpl>()->getResource();
    }


    //@―---------------------------------------------------------------------------
    //! @brief      バッファへコピー
    //@―---------------------------------------------------------------------------
    void DisplayImpl::recordApplyDisplay(CommandListImpl& cmdList) {
        
        if (!m_bindedTexture)
            return;

        cmdList.beginRenderPass(m_buffers.current());
        cmdList.setPipelineState(m_pipeline);

        {
            SetDescriptorTableParam param(m_bindedTextureTable, 0);
            cmdList.setRootDesciptorTable(&param, 1);
        }
        cmdList.setVertexBuffer(m_verices);
        {
            DrawParam param;
            param.startVertex = 0;
            param.vertexCount = 6;
            cmdList.draw(param);
        }


        if (auto texture = m_textures.current().cast<TextureImpl>()) {

            D3D12_RESOURCE_BARRIER barrier;
            if (texture->addResourceTransition(barrier, D3D12_RESOURCE_STATE_PRESENT)) {
                cmdList.getNative()->ResourceBarrier(1, &barrier);
            }

        }

    }

}// namespace ob::rhi::dx12
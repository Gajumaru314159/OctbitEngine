//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Forward.h>
#include <Framework/Graphics/Material/MaterialDesc.h>
#include <Framework/Graphics/Material/MaterialSystem.h>

namespace ob::graphics {   

    class MaterialBlock;

    //! @brief  マテリアル描画コマンドを記録するラッパークラス
    class MaterialRecorder {
    public:
        MaterialRecorder(Ref<rhi::CommandList>& cmd, MaterialRenderer& renderer);
        MaterialRecorder(Ref<rhi::CommandList>& cmd, RenderScene& scene);
        MaterialRecorder(Ref<rhi::CommandList>& cmd);

        void record(Ref<Mesh>& mesh, s32 submesh, Ref<Material2>& material, StringView pass);
    private:
		MaterialBlockSet m_blocks;
		Ref<rhi::CommandList> m_cmdList;
    };

}
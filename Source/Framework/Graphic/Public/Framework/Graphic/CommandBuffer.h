//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/String/Name.h>
#include <Framework/RHI/Buffer.h>

namespace ob::graphic {

    class Texture;
    class Material;
    class Mesh;

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //! 
    //! @details    
    //@―---------------------------------------------------------------------------
    class CommandBuffer {
    public:
        void clear();
        void blit(const Texture& source, Texture& dest, const Material& mat);// scale/offset/TextureArrayIndex
        // void copyTexture(src,dest);// フォーマット/サイズが違う場合はレンダー   領域

        void drawMesh(const Mesh& mesh, const Matrix&, const Material&, Name shaderPass);
        void drawMeshInstanced(const Mesh& mesh, Span<Matrix>, const Material&, Name shaderPass);

        void drawMesh(Span<rhi::Buffer> streams,const rhi::Buffer& indices, const Matrix&, const Material&, Name shaderPass);


        void getTemporaryRT();

    private:
        Pimpl<class CommandBufferImpl> m_impl;
    };


}// namespcae ob
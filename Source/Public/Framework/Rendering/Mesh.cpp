//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Rendering/Mesh.h>
#include <Framework/Rendering/Private/MeshImpl.h>

namespace ob::rendering {

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------

    Mesh::Mesh() {

    }
    Mesh::Mesh(const MeshData& meshData) {
        m_impl = std::make_shared<MeshImpl>();
    }

    size_t Mesh::num_vertices()const {
        if (!m_impl)return 0;
        return m_impl->num_vertices();
    }
    size_t Mesh::num_triangles()const {
        if (!m_impl)return 0;
        return m_impl->num_vertices();
    }
    Box Mesh::boundingBox()const {
        if (!m_impl)return Box(Vec3::Zero);
        return m_impl->boundingBox();
    }
    Sphere Mesh::boundingSphere()const {
        if (!m_impl)return Sphere(Vec3::Zero,0.0f);
        return m_impl->boundingSphere();
    }

    void Mesh::draw()const {
        if(m_impl)m_impl->draw();
    }

}// namespace ob
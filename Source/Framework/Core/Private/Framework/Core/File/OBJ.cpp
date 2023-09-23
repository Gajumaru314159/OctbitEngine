//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/File/OBJ.h>
#include <Framework/Core/File/FileStream.h>
#include <fstream>
#include <sstream>

namespace ob::core {

	void OBJ::save(StringView path) {
		std::ofstream os{std::string(path.data(), path.size())};
		os << m_ss.str();
	}

	//@―---------------------------------------------------------------------------
	//! @brief  コメントを追加
	//@―---------------------------------------------------------------------------
	void OBJ::comment(StringView comment) {
		m_ss << "# " << comment << std::endl;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  マテリアル参照を追加
	//@―---------------------------------------------------------------------------
	void OBJ::mtl(StringView filename) {
		m_ss << "mtllib " << filename << std::endl;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  マテリアルを設定
	//@―---------------------------------------------------------------------------
	void OBJ::material(StringView material) {
		m_ss << "usemtl " << material << std::endl;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  オブジェクト名を設定
	//@―---------------------------------------------------------------------------
	void OBJ::name(StringView name) {
		m_ss << "o " << name << std::endl;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  グループ名を設定
	//@―---------------------------------------------------------------------------
	void OBJ::group(StringView group) {
		m_ss << "g " << group << std::endl;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  座標を追加
	//@―---------------------------------------------------------------------------
	s32 OBJ::position(Vec3 p) {
		m_ss << "v " << p.x << " " << p.y << " " << p.z << std::endl;
		return ++m_pcount;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  UV座標を追加
	//@―---------------------------------------------------------------------------
	s32 OBJ::texcoord(Vec2 uv) {
		m_ss << "vn " << uv.x << " " << uv.y << std::endl;
		return ++m_tcount;
	}

	//@―---------------------------------------------------------------------------
	//! @brief  法線を追加
	//@―---------------------------------------------------------------------------
	s32 OBJ::normal(Vec3 n) {
		m_ss << "vn " << n.x << " " << n.y << " " << n.z << std::endl;
		return ++m_ncount;
	}

	void OBJ::smoothLevel(s32 level) {
		if (level == 0) {
			m_ss << "s off" << std::endl;
		} else {
			m_ss << "s " << level << std::endl;
		}
	}

	s32 OBJ::pcount()const {
		return m_pcount;
	}
	s32 OBJ::tcount()const {
		return m_tcount;
	}
	s32 OBJ::ncount()const {
		return m_ncount;
	}


	void OBJ::face_p_internal(s32 count) {
		m_ss << std::endl;
	}
	void OBJ::face_pt_internal(s32 count) {
		m_ss << std::endl;
	}
	void OBJ::face_pn_internal(s32 count) {
		m_ss << std::endl;
	}
	void OBJ::face_ptn_internal(s32 count) {
		m_ss << std::endl;
	}
	void OBJ::line_internal(s32 count) {
		m_ss << std::endl;
	}


	void OBJ::poly_p_internal(s32 count) {
		m_ss << "f";
		for (s32 i = count - 1; 0 <= i; --i) {
			m_ss << " " << m_pcount - i;
		}
		m_ss << std::endl;
	}
	void OBJ::poly_pt_internal(s32 count) {
		m_ss << "f";
		for (s32 i = count - 1; 0 <= i; --i) {
			m_ss << " " << m_pcount - i << "/" << m_tcount - i;
		}
		m_ss << std::endl;
	}
	void OBJ::poly_pn_internal(s32 count) {
		m_ss << "f";
		for (s32 i = count - 1; 0 <= i; --i) {
			m_ss << " " << m_pcount - i << "//" << m_ncount - i;
		}
		m_ss << std::endl;
	}
	void OBJ::poly_ptn_internal(s32 count) {
		m_ss << "f";
		for (s32 i = count - 1; 0 <= i; --i) {
			m_ss << " " << m_pcount - i << "/" << m_tcount - i << "/" << m_ncount - i;
		}
		m_ss << std::endl;
	}
	void OBJ::poly_line_internal(s32 count) {
		m_ss << "l";
		for (s32 i = count - 1; 0 <= i; --i) {
			m_ss << " " << m_pcount - i;
		}
		m_ss << std::endl;
	}




	void MTL::save(StringView path) {
		std::ofstream os{std::string(path.data(), path.size())};
		os << m_ss.str();
	}
	void MTL::newmtl(StringView name) {
		m_ss << "newmtl " << name << std::endl;
	}
	void MTL::ambient(Color color, StringView tex) {
		m_ss << "Ka " << color.r << " " << color.g << " " << color.b << std::endl;
		if (tex.empty())return;
		m_ss << "map_Ka " << tex << std::endl;
	}
	void MTL::diffuse(Color color, StringView tex) {
		m_ss << "Kd " << color.r << " " << color.g << " " << color.b << std::endl;
		if (tex.empty())return;
		m_ss << "map_Kd " << tex << std::endl;
	}
	void MTL::specular(Color color, StringView tex) {
		m_ss << "Ks " << color.r << " " << color.g << " " << color.b << std::endl;
		if (tex.empty())return;
		m_ss << "map_Ks " << tex << std::endl;
	}
	void MTL::shininess(f32 ns, StringView tex) {
		m_ss << "Ns " << ns << std::endl;
		if (tex.empty())return;
		m_ss << "map_Ns " << tex << std::endl;
	}
	void MTL::alpha(f32 alpha, StringView tex) {
		m_ss << "d " << alpha << std::endl;
		if (tex.empty())return;
		m_ss << "map_d " << tex << std::endl;
	}
	void MTL::illumination(s32 mode) {
		m_ss << "illum" << mode << std::endl;
	}
	void MTL::ior(f32 ior) {
		m_ss << "Ni " << ior << std::endl;
	}
	void MTL::bump(StringView tex) {
		if (tex.empty())return;
		m_ss << "map_bump " << tex << std::endl;
	}
	void MTL::displacement(StringView tex) {
		if (tex.empty())return;
		m_ss << "disp " << tex << std::endl;
	}
	void MTL::decal(StringView tex) {
		if (tex.empty())return;
		m_ss << "decal " << tex << std::endl;
	}

}
//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Core/Core.h>

namespace ob::core {

	//@―---------------------------------------------------------------------------
	//! @brief		Wavefront Obj
	//! @details	Vertex/Texcoord/Normal
	//@―---------------------------------------------------------------------------
	class OBJ {
	public:
		struct Vertex {
			Vec3 position;
			Vec3 normal;
			Vec2 uv;
		};
		enum class VFormat {
			P,
			PT,
			PN,
			PTN,
		};
	public:
		OBJ() = default;

		void save(StringView);

		void comment(StringView comment);
		void mtl(StringView filename);
		void material(StringView name);
		void group(StringView);
		void name(StringView);

		s32 position(Vec3);
		s32 texcoord(Vec2);
		s32 normal(Vec3);

		void smoothLevel(s32 level);

		s32 pcount()const;
		s32 tcount()const;
		s32 ncount()const;

		template<class ...Args> void face_p(Args... args) { face_p_internal(0, args...); }
		template<class ...Args> void face_pt(Args... args) { face_pt_internal(0, args...); }
		template<class ...Args> void face_pn(Args... args) { face_pn_internal(0, args...); }
		template<class ...Args> void face_ptn(Args... args) { face_ptn_internal(0, args...); }
		template<class ...Args> void line(Args... args) { line_internal(0, args...); }

		template<class ...Args> void poly_p(Args... args) { poly_p_internal(0, args...); }
		template<class ...Args> void poly_pt(Args... args) { poly_pt_internal(0, args...); }
		template<class ...Args> void poly_pn(Args... args) { poly_pn_internal(0, args...); }
		template<class ...Args> void poly_line(Args... args) { poly_line_internal(0, args...); }

	private:

		void face_p_internal(s32 count);
		void face_pt_internal(s32 count);
		void face_pn_internal(s32 count);
		void face_ptn_internal(s32 count);
		void line_internal(s32 count);

		template<class ...Args>
		void face_p_internal(s32 count, s32 p, Args... args) {
			if (count == 0) m_ss << "f";
			m_ss << " " << p;
			face_p_internal(count + 1, args...);
		}

		template<class ...Args>
		void face_pt_internal(s32 count, s32 p, s32 t, Args... args) {
			if (count == 0) m_ss << "f";
			m_ss << " " << p << "/" << t;
			face_pt_internal(count + 1, args...);
		}

		template<class ...Args>
		void face_pn_internal(s32 count, s32 p, s32 n, Args... args) {
			if (count == 0) m_ss << "f";
			m_ss << " " << p << "//" << t;
			face_pn_internal(count + 1, args...);
		}

		template<class ...Args>
		void face_ptn_internal(s32 count, s32 p, s32 t, s32 n, Args... args) {
			if (count == 0) m_ss << "f";
			m_ss << " " << p << "/" << t << "/" << n;
			face_pn_internal(count + 1, args...);
		}

		template<class ...Args>
		void line_internal(s32 count, s32 p, Args... args) {
			if (count == 0) m_ss << "l";
			m_ss << " " << p;
			line_internal(count + 1, args...);
		}

		void poly_p_internal(s32 count);
		void poly_pt_internal(s32 count);
		void poly_pn_internal(s32 count);
		void poly_ptn_internal(s32 count);
		void poly_line_internal(s32 count);

		template<class ...Args>
		void poly_p_internal(s32 count, Vec3 p, Args... args) {
			position(p);
			poly_p_internal(count + 1, args...);
		}

		template<class ...Args>
		void poly_pt_internal(s32 count, Vec3 p, Vec2 t, Args... args) {
			position(p);
			texcoord(t);
			poly_pt_internal(count + 1, args...);
		}

		template<class ...Args>
		void poly_pn_internal(s32 count, Vec3 p, Vec3 n, Args... args) {
			position(p);
			normal(n);
			poly_pn_internal(count + 1, args...);
		}

		template<class ...Args>
		void poly_ptn_internal(s32 count, Vec3 p, Vec2 t, Vec3 n, Args... args) {
			position(p);
			texcoord(t);
			normal(n);
			poly_ptn_internal(count + 1, args...);
		}

		template<class ...Args>
		void poly_line_internal(s32 count, Vec3 p, Args... args) {
			position(p);
			poly_line_internal(count + 1, args...);
		}

	private:
		std::stringstream m_ss;
		s32 m_pcount = 0;
		s32 m_tcount = 0;
		s32 m_ncount = 0;
	};


	class MTL {
	public:
		struct TextureOption {
			Vec2 offset{0,0};
			Vec2 scale{1,1};
		};
	public:
		void save(StringView);

		void newmtl(StringView name);
		void ambient(Color color = {0.2,0.2,0.2}, StringView tex = "");
		void diffuse(Color color = {0.8,0.8,0.8}, StringView tex = "");
		void specular(Color color = {1.0,1.0,1.0}, StringView tex = "");
		void shininess(f32 ns = 0.0f, StringView tex = "");
		void alpha(f32 alpha, StringView tex = "");
		void illumination(s32);
		void ior(f32);
		void bump(StringView tex);
		void displacement(StringView tex);
		void decal(StringView tex);
	private:
		std::stringstream m_ss;
	};

}
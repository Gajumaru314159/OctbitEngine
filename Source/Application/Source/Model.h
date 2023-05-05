#include <Framework/RHI/All.h>

#include <Framework/Platform/Window.h>

#include <Framework/Input/All.h>
#include <Framework/Input/Config.h>

#include <Framework/Engine/Engine.h>
#include <Plugins/ImGui/ImGui.h>

#include <Framework/Engine/Scene.h>
#include <Framework/Engine/Entity.h>

#include <Framework/Graphics/Material.h>
#include <Framework/Graphics/Mesh.h>
#include <Framework/Graphics/CommandBuffer.h>


#pragma warning(push, 0)
#include <OBJ_Loader.h>
#pragma warning(pop)

//-----------------------------------------------------------------
using namespace ob;
using namespace ob::rhi;
using namespace ob::graphics;


class Model {
public:

	Model(StringView obj,StringView tex) {
		initMesh(obj);
		initTexture(tex);
		initMaterial();
	}

	void draw(Ref<CommandBuffer> cmdBuf) {
		for (s32 i = 0; i < m_mesh->getSubMeshCount(); ++i) {
			cmdBuf->drawMesh(m_mesh, i, Matrix::Identity, m_material, Name(TC("Opaque")));
		}
	}

	void setMatrix(const Matrix& matrix) {
		m_material->setMatrix(TC("Matrix"), matrix);
	}

private:
	void initMesh(StringView obj) {
		
		objl::Loader Loader;

		U8String path;
		StringEncoder::Encode(obj, path);
		
		if (Loader.LoadFile(path.c_str())) {

			LOG_INFO("モデル読み込み [{}]",obj);

			MeshData meshData;
			size_t indexStart = 0;
			size_t base = 0;

			for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
			{
				objl::Mesh& curMesh = Loader.LoadedMeshes[i];

				String name;
				StringEncoder::Encode(curMesh.MeshName, name);

				LOG_INFO("    mesh={}", name);

				for (int j = 0; j < curMesh.Vertices.size(); j++)
				{
					auto pos = curMesh.Vertices[j].Position;
					auto uv = curMesh.Vertices[j].TextureCoordinate;
					auto normal = curMesh.Vertices[j].Normal;

					meshData.positions.emplace_back(pos.X, pos.Y, pos.Z);
					meshData.tangents.emplace_back(normal.X, normal.Y, normal.Z,0.f);
					meshData.normals.emplace_back(normal.X, normal.Y, normal.Z);
					meshData.uvs.emplace_back(uv.X, 1-uv.Y);
				}

				for (int j = 0; j < curMesh.Indices.size(); j++)
				{
					meshData.indices.push_back(curMesh.Indices[j] + base);
				}

				SubMesh submesh;
				submesh.indexStart = base;
				submesh.indexCount = curMesh.Indices.size();

				meshData.submeshes.push_back(submesh);

				indexStart += curMesh.Indices.size();
				base += curMesh.Vertices.size();

			}

			meshData.name = path;
			m_mesh = Mesh::Create(meshData);


		} else {
			LOG_INFO("モデルファイルが見つかりませんでした。[{}]", obj);
		}

	}
	void initTexture(StringView name) {

		m_texture = Texture::Load(name.data());
		OB_ASSERT_EXPR(m_texture);

	}

	void initMaterial() {

		Ref<RootSignature> signature;
		{
			RootSignatureDesc desc(
				{
					RootParameter::Range(DescriptorRangeType::CBV,1,0),		// バッファ
					RootParameter::Range(DescriptorRangeType::SRV,1,0),		// テクスチャ
					//RootParameter::Range(DescriptorRangeType::Sampler,1,0),	// サンプラー
					RootParameter::Range(DescriptorRangeType::CBV,1,1),		// バッファ
					RootParameter::Range(DescriptorRangeType::SRV,1,1),		// テクスチャ
					//RootParameter::Range(DescriptorRangeType::Sampler,1,1),	// サンプラー
				},
			{
				StaticSamplerDesc(SamplerDesc(TextureFillter::Point),0),
			}
			);
			desc.name = TC("Common");
			signature = RootSignature::Create(desc);
			OB_ASSERT_EXPR(signature);
		}


		Ref<Shader> vs;
		Ref<Shader> ps;
		{
			String code=TC(R"(

SamplerState g_mainSampler:register(s0);

// Global
Texture2D s_skyTex:register(t0);
cbuffer Param : register(b0) {
  //float    s_scalars[4];
  float4   s_colors[1];
  float4x4 s_matrices[1];
};

// Local
Texture2D g_mainTex:register(t1);
cbuffer Param : register(b1) { 
  //float    g_scalars[4];
  float4   g_colors[1];
  float4x4 g_matrices[1];
};

// IN / OUT
struct VsIn {
  float4 pos	:POSITION;
  float4 normal	:NORMAL;
  float2 uv		:TEXCOORD;
};
struct PsIn {
  float4 pos	:SV_POSITION;
  float4 normal	:NORMAL;
  float2 uv		:TEXCOORD;
};
struct PsOut {
  float4 color0	:SV_TARGET0;
  float4 color1	:SV_TARGET1;
};
// エントリ
PsIn VS_Main(VsIn i) {
    PsIn o;
    o.pos = mul(s_matrices[0],mul(g_matrices[0],float4(i.pos.xyz,1)));
    o.uv = i.uv;
    o.normal = i.normal;
    return o;
}
PsOut PS_Main(PsIn i){
    PsOut o;
    float4 color = g_mainTex.Sample(g_mainSampler,i.uv) * g_colors[0];
	color.rgb *= (dot(i.normal.xyz,s_colors[0].xyz)*0.25+0.25+0.5);
    o.color1 = color;o.color1.xyz=1-o.color1.xyz;
    o.color0 = color;
    return o;
}
			)");

			vs = Shader::CompileVS(code);
			ps = Shader::CompilePS(code);
			OB_ASSERT_EXPR(vs && ps);
		}
		
		{
			MaterialDesc desc;
			desc.colorProperties.emplace_back(TC("Color"));
			desc.matrixProperties.emplace_back(TC("Matrix"));
			desc.textureProperties.emplace_back(TC("Main"));

			MaterialPass pass;
			pass.renderTag = TC("Opaque");
			pass.layout = m_mesh->getVertexLayout();
			pass.rasterizer.cullMode = CullMode::None;
			pass.depthStencil.depth.enable = true;
			pass.depthStencil.stencil.enable = false;
			pass.vs = vs;
			pass.ps = ps;
			pass.rootSignature = signature;
			desc.passes.emplace(pass.renderTag, pass);

			m_material = Material::Create(desc);

			m_material->setColor(TC("Color"), Color::White);
			m_material->setMatrix(TC("Matrix"), Matrix::Identity);
			m_material->setTexture(TC("Main"), m_texture);
		}
	}

private:
	Ref<Texture> m_texture;
	Ref<Mesh> m_mesh;
	Ref<Material> m_material;

};
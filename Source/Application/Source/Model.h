#include <Windows.h>

#include <Framework/RHI/All.h>

#include <Framework/Platform/Window.h>

#include <Framework/Input/All.h>
#include <Framework/Input/Config.h>

#include <Framework/Engine/Engine.h>
#include <Framework/Engine/EngineConfig.h>
#include <Plugins/ImGui/ImGui.h>

#include <Framework/Engine/Scene.h>
#include <Framework/Engine/Entity.h>

#include <Framework/Graphic/Material.h>
#include <Framework/Graphic/Mesh.h>
#include <Framework/Graphic/CommandBuffer.h>


#pragma warning(push, 0)
#include <OBJ_Loader.h>
#pragma warning(pop)

//-----------------------------------------------------------------
using namespace ob;
using namespace ob::rhi;
using namespace ob::graphic;


class Model {
public:

	Model(const char* name) {
		initMesh(name);
		initTexture();
		initMaterial();
	}

	void draw(Ref<CommandBuffer> cmdBuf) {
		cmdBuf->drawMesh(m_mesh, 0, Matrix::Identity, m_material, engine::Name(TC("Opaque")));
	}

	void setMatrix(const Matrix& matrix) {
		m_material->setMatrix(TC("Matrix"), matrix);
		m_material->setColor(TC("Color"), Color::Red);
	}

private:
	void initMesh(const char* name) {
		
		objl::Loader Loader;
		
		if (Loader.LoadFile(name)) {
			LOG_INFO("モデル読み込み");


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
					meshData.normals.emplace_back(normal.X, normal.Y, normal.Z);
					meshData.uvs.emplace_back(uv.X, uv.Y);
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

			meshData.name = TC("Monky");
			m_mesh = Mesh::Create(meshData);


		} else {
			LOG_INFO("モデルファイルが見つかりませんでした。");
		}

	}
	void initTexture() {

		m_texture = Texture::Load(TC("Asset/Texture/sky.dds"));
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
				},
			{
				StaticSamplerDesc(SamplerDesc(),0),
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
Texture2D g_mainTex:register(t0);							
cbuffer Param : register(b0) {								
  float4 g_col;
  float4x4 g_mtx;
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
    o.pos = mul(g_mtx,i.pos);								
    o.uv = i.uv;											
    o.normal = i.normal;									
    return o;												
}															
PsOut PS_Main(PsIn i){										
    PsOut o;												
    float4 color = g_mainTex.Sample(g_mainSampler,i.uv);	
    o.color1 = i.normal;									
    o.color0 = color;										
    return o;												
}								
			)");

			vs = VertexShader::Create(code);
			ps = PixelShader::Create(code);
			OB_ASSERT_EXPR(vs && ps);
		}
		
		{
			MaterialDesc desc;
			desc.colorProperties.emplace_back(TC("Color"), 0);
			desc.matrixProperties.emplace_back(TC("Matrix"), 16);
			desc.textureProperties.emplace_back(TC("Main"), 0);

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

			m_material->setColor(TC("Color"), Color::Red);
			m_material->setMatrix(TC("Matrix"), Matrix::Identity);
			m_material->setTexture(TC("Main"), m_texture);
		}
	}

private:
	Ref<Texture> m_texture;
	Ref<Mesh> m_mesh;
	Ref<Material> m_material;

};
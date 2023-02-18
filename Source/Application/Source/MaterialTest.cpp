/*
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


void TestMaterial() {

	Ref<Mesh> mesh;

	// Initialize Loader
	objl::Loader Loader;

	// Load .obj File
	if (Loader.LoadFile("Asset/Model/Monky.obj")) {
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
				meshData.indices.push_back(curMesh.Indices[j]+base);
			}

			SubMesh submesh;
			submesh.indexStart = base;
			submesh.indexCount = curMesh.Indices.size();
			
			meshData.submeshes.push_back(submesh);

			indexStart += curMesh.Indices.size();
			base += curMesh.Vertices.size();

		}

		meshData.name = TC("Monky");
		mesh = Mesh::Create(meshData);


	} else {
		LOG_INFO("モデルファイルが見つかりませんでした。");
	}

	{
		MaterialDesc desc;
		MaterialPass pass;
		//pass.vs = ;
		//auto& pass = desc.passes.emplace(TC("Opaque"));
		//pass->
	}



	auto commandBuffer = CommandBuffer::Create();

	//commandBuffer->drawMesh(mesh, 0, Matrix::Identity, material, TC(""));


}
*/
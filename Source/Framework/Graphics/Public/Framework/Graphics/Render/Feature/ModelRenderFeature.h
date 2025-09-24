//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Graphics/Render/RenderView.h>
#include "Framework/Graphics/Material/Material.h"
#include "Framework/RHI/CommandList.h"
#if false
namespace ob::graphics {

	struct CullResult {
		bool culled;
	};
	struct LodResult {
		s32 lod;
	};

	struct ViewData {
		Vector<CullResult> culls;
		Vector<LodResult> lods;
	};

	class BatchRenderFeature
	{
	public:
		void render(RenderView& view,const String& passName,Ref<rhi::CommandList>& cmd)
		{
			view.drawList;
		}

		void addBatch()
		{

		}
	};

	struct MeshDesc
	{
		Ref<Mesh> mesh;
		Vector<Ref<Material>> materials;
	};

	struct UnitKey
	{
		Ref<Mesh> mesh;
		Vector<Ref<Material>> materials;

		bool operator==(const UnitKey& rhs) const
		{
			if (mesh != rhs.mesh) return false;
			if (materials.size() != rhs.materials.size()) return false;
			for (s32 i = 0; i < materials.size(); ++i)
			{
				if (materials[i] != rhs.materials[i]) return false;
			}
			return true;
		}
	};
	struct UnitKeyHasher
	{
		size_t operator()(const UnitKey& key) const
		{
			size_t hash = 0;
			return hash;
		}
	};

	//! @brief      マテリアル描画機能
	class ModelRenderFeature : public RenderFeature {
	private:
		enum class Strategy {
			CPU,
			CPU_GPU,
			GPU
		};
		struct Strategies {
			Strategy lod = Strategy::CPU;
			Strategy cull = Strategy::CPU;
			Strategy args = Strategy::CPU;
		};

		struct Bounds {

		};

		struct Handle {
			// s32& operator[](size_t index) { }

			s32 size() const { return m_size; }
			s32 offset() const { return m_offset; }
			s32 m_offset;
			s32 m_size;
		};

		struct Lod {
			Handle instances;
			Handle instanceCount;;
		};
		struct Section {
		};
		struct Bin {
			Handle args;
			Handle argsCount;
			Vector<Section> sections;
			Ref<Material> material;
		};
		struct Unit {
			Handle instances;
			Vector<Lod> lods;
			Vector<Bin> bins;
		};

		class FrameAllocator {
		public:
			Handle alloc(size_t size) {
				Handle handle{(s32)m_size};
				m_size += size;
				return handle;
			}
		private:
			using Buffer = rhi::Buffer;
		private:
			size_t m_size = 0;
			Vector<s32> m_cpu;
			Ref<Buffer> m_gpu;
		};

		FrameAllocator m_instanceAllocator;
		FrameAllocator m_instanceCountAllocator;
		FrameAllocator m_argsAllocator;
		FrameAllocator m_countAllocator;

		template<class T>
		class DenseMap {

		};

	public:

		OB_RTTI();

		ModelRenderFeature(RenderScene& scene) {

		}

		void preRender() {

			// 前フレームの追加データを反映


			// 毎フレームアロケートではないのでこの実装ではない
			for(auto& unit:m_units) {
				unit.instances = alloc(unit.instances.size());
				for(auto& lod:unit.lods) {
					lod.instances = m_instanceAllocator.alloc(unit.instances.size());
					lod.instanceCount = m_instanceCountAllocator.alloc(1);
				}
				for(auto& bin:unit.bins) {
					bin.args = m_argsAllocator.alloc(bin.sections.size());
					bin.argsCount = m_countAllocator.alloc(1);
				}
			}


		}

		void render(RenderView& view) {
			// LOD  : CPUのみ CPU計算GPUアップ GPU計算
			// Cull : CPUのみ CPU計算GPUアップ GPU計算
			// Args : CPUのみ CPU計算GPUアップ GPU計算

			auto& data = view.get<ViewData>();

			if (m_strategies.lod == Strategy::GPU) {
				// dispatchLOD();
			} else {
				data.lods.clear();
				data.lods.reserve(m_bounds.size());;
				for(const auto& [i,bounds]:Indexed(m_bounds)) {
					data.lods[i].lod = 0;// CalcLod(view,bounds);
				}
			}


			if (m_strategies.cull == Strategy::GPU) {
				// dispatchCull();
			} else {
				data.lods.clear();
				data.lods.reserve(m_bounds.size());;
				for(const auto& [i,bounds]:Indexed(m_bounds)) {
					data.culls[i].culled = false;// CalcCull(view,bounds);
				}
			}


			if (m_strategies.args == Strategy::GPU) {

			} else {
				for(auto& unit:m_units) {
					// ISMの場合は追加で全インスタンスを包含するBoundsを持つ
					// インスタンス列は共通で間接描画引数はSection毎に持つ
					for(auto& instance:unit.instances) {

					}
					for(auto& bin:unit.bins) {
						for(auto& section:bin.sections) {
							// インスタンス列と描画時引数を生成
							// インスタンス列はGPU必須（もしくは1つずつ描画でGPU送信なし）
							// 引数はCPUで送信
						}
					}
				}
			}

			// バッチ生成
		}

		void render(RenderView& view,const String& passName,Ref<rhi::CommandList>& cmd) {

			auto& data = view.get<ViewData>();
			auto& units = m_unitMap[passName];

			for(auto& iUnit:units) {

				auto& unit = m_units[iUnit];

				for(auto& bin:unit.bins) {

					// TODO 使いたいVertexLayoutのPipelineStateを設定
					// bin.material->record(cmd,blocks,bin.mesh,passName);
					// bin.mesh->record(cmd);

					if(m_strategies.args == Strategy::CPU) {
						for(auto& section:bin.sections) {
							for(auto& instance:unit.instances) {
								if(data.culls[instance].culled) continue;
								// cmd->drawIndexedInstanced(mesh,instance,1);
							}
						}
					} else if(m_strategies.args == Strategy::CPU_GPU) {
						for(auto& section:bin.sections) {
							// cmd->drawIndexedInstancedIndirect(view.argsBuffer[bin.args],view.argsCountBuffer[bin.args]);
						}
					} else {
						// setup(visibleInstances);
						// draw(bin.geometry,bin.material,bin.args,bin.count);
						// cmd->drawIndexedInstancedIndirect(m_argsBuffer[bin.args],m_argsCountBuffer[bin.args]);
					}

				}
			}

		}



		s32 acuireMesh(const MeshDesc& desc)
		{
			UnitKey key;
			key.mesh = desc.mesh;
			key.materials = desc.materials;

			auto [itr,added] = m_units2.emplace(key);
			Unit& unit = *itr;

			if (added)
			{
				unit.lods.resize(desc.mesh->lods.size());
				unit.bins.resize(desc.materials.size());
			}

			unit.instances.emplace_back();

			return 0;
		}

	private:
		Strategies m_strategies;
		Vector<Bounds> m_bounds;
		Vector<Unit> m_units;

		HashMap<UnitKey,Unit,UnitKeyHasher> m_units2;
	};

	void sample() {
		if(ModelRenderFeature* feature = findFeature<ModelRenderFeature>()) {

			SkeletalMeshHandle handle;

			MeshDesc desc;
			desc.mesh = m_mesh;
			desc.materials = m_materials;

			handle = feature->acuireMesh(desc);

			feature->setTransform(handle,transform);
		}
	}
}
#endif

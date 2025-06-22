//***********************************************************
//! @file
//! @brief		
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphics/Render/RenderFeature.h>
#include <Framework/Core/Template/Container/IndexedVector.h>
#include <Framework/RHI/Buffer.h>
#include <Framework/Graphics/Render/Feature/MaterialRenderFeature.h>
#include <Framework/Graphics/Material/MaterialPropertiesDesc.h>
#include <Framework/Graphics/Material/Material.h>
namespace ob::graphics {

	struct DirectionalLightData {
		Color color;
		Vec3 direction;
		f32  intensity;
	};
	struct PointLightData {
		Color color;
		Vec3 position;
		f32  intensity;
	};
	struct SpotLightData {
		Color color;
		Vec3 position;
		f32  intensity;
		Vec3 direction;
		f32  anguler;
	};

	class ResizableBuffer {
	public:
		struct Desc {
			s32 stride;
		};
	public:
		ResizableBuffer(const Desc& desc)
			: m_desc(desc), m_size(0)
		{
			resize(16);
		}

		void resize(size_t size) {
			m_size = size;
			m_buffer = rhi::Buffer::Create(rhi::BufferDesc::ByteAddress(m_desc.stride * m_size));
		}

		void update(size_t count, const void* data) {
			using namespace ob::rhi;
			if (m_size <= count) {
				resize(count * 2);
			}
			m_buffer->update(m_desc.stride * count, data);
		}

		Ref<rhi::Buffer> getBuffer() const {
			return m_buffer;
		}
	private:
		Desc m_desc;
		Ref<rhi::Buffer> m_buffer;
		size_t m_size;
	};


	//! @brief      マテリアル描画機能
	class PointLightRenderFeature : public RenderFeature {
	public:
		static MaterialPropertiesSetDesc GetProperties() {
			MaterialPropertiesSetDesc desc;
			desc.scene.buffers = {
				"PointLight"
			};
			return desc;
		}
	public:

		OB_RTTI();

		PointLightRenderFeature()
			: m_buffer({ sizeof(PointLightData) })
		{

		}

		s32 acquire() {
			return m_point.push();
		}
		void release(s32 index) {
			m_point.erase(index);
		}
		void set(s32 index, const PointLightData& data) {
			if (index < 0 || index >= m_point.size()) {
				LOG_FATAL("PointLightRenderFeature::set: Invalid index {}", index);
				return;
			}
			m_point.at(index) = data;
		}

		void render(FG& fg,RenderScene& scene) {
			m_buffer.update(m_point.size(), m_point.data());

			if (auto feature = scene.findFeature<MaterialRenderFeature>()) {
				feature->getSceneBlock().setBuffer("PointLight", m_buffer.getBuffer());
			}
		}

	private:
		IndexedVector<PointLightData> m_point;
		ResizableBuffer m_buffer;
	};

}
//***********************************************************
//! @file
//! @brief		プリコンパイルヘッダ
//! @author		Gajumaru
//***********************************************************
#include <Framework/Model/Model.h>

namespace ob::model {

	class ModelInstance {
	public:
		ModelInstance(RenderScene& scene) 
			: m_scene(scene)
		{

		}

		void setActive(bool value) {
			if (value) {
				m_scene.addTask(m_hTask, {*this,&ModelInstance::render});
			} else {

			}
		}
	private:
		void render() {

		}
	private:
		RenderScene m_scene;
		RenderTaskHandle m_hTask;
	};

}
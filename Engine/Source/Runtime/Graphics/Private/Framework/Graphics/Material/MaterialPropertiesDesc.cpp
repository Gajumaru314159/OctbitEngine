//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Material/MaterialPropertiesDesc.h>

namespace ob::graphics {

	//! @brief プロパティ名の重複をチェックしながらプロパティを追加する
	static bool Merge(Vector<String>& dst, const Vector<String>& src) {
		bool success = true;
		for (const auto& name : src) {
			if (std::find(dst.begin(), dst.end(), name) == dst.end()) {
				dst.push_back(name);
			}
			else {
				LOG_ERROR("既に登録されているプロパティです [{}]", name);
				success = false;
			}
		}
		return success;
	}

	//! @brief 別の MaterialPropertiesDesc を各プロパティリストの後に追加する
	bool MaterialPropertiesDesc::merge(const MaterialPropertiesDesc& other) {
		bool success = true;
		success &= Merge(textures, other.textures);
		success &= Merge(buffers, other.buffers);
		success &= Merge(matrices, other.matrices);
		success &= Merge(vectors, other.vectors);
		success &= Merge(scalars, other.scalars);
		success &= Merge(integers, other.integers);
		return success;
	}

	//! @brief 別の MaterialPropertiesSetDesc を各プロパティリストの後に追加する
	bool MaterialPropertiesSetDesc::merge(const MaterialPropertiesSetDesc& other) {
		bool success = true;
		success &= global.merge(other.global);
		success &= scene.merge(other.scene);
		success &= view.merge(other.view);
		return success;
	}
}
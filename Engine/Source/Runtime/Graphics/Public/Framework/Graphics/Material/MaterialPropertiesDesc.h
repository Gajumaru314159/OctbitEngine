//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::graphics {

	struct MaterialPropertiesDesc {
		Vector<String>  textures;
		Vector<String>  buffers;
		Vector<String>  matrices;
		Vector<String>  vectors;
		Vector<String>  scalars;
		Vector<String>  integers;

		//! @brief 別の MaterialPropertiesDesc を各プロパティリストの後に追加する
		//! @retval true 成功
		//! @retval false プロパティ名の重複が含まれていない
		bool merge(const MaterialPropertiesDesc& other);
	};

	struct MaterialPropertiesSetDesc {
		MaterialPropertiesDesc global;
		MaterialPropertiesDesc scene;
		MaterialPropertiesDesc view;

		//! @brief 別の MaterialPropertiesSetDesc を各プロパティリストの後に追加する
		bool merge(const MaterialPropertiesSetDesc& other);
	};

}
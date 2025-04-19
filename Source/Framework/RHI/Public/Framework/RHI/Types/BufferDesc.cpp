//***********************************************************
//! @file
//! @brief		バッファ定義
//! @author		Gajumaru
//***********************************************************
#include <Framework/RHI/Types/BufferDesc.h>

namespace ob::rhi {

	//! @brief バリデート
	bool BufferDesc::isValid() const {

		if (size == 0) {
			LOG_ERROR("バッファサイズは0より大きくなくてはいけません。[name={}]", name);
			return false;
		}

		if (state == BufferState::Constant && size % 256 != 0) {
			LOG_ERROR("定数バッファは256の倍数で作成する必要があります。 [name={},size]", name, size);
			return false;
		}

		if (size < 65536 && size % 4 != 0) {
			LOG_ERROR("64KiB以下のバッファサイズは4の倍数である必要があります。 [name={} size={}]", name, size);
			return false;
		}

		return true;
	}


}
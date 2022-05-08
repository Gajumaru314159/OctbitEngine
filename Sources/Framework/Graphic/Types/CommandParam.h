//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once
#include <Framework/Graphic/DescriptorTable.h>

namespace ob::graphic {

	//@―---------------------------------------------------------------------------
	//! @brief  デスクリプタ・テーブル・コマンドパラメータ
	//@―---------------------------------------------------------------------------
	struct SetDescriptorTableParam {
		DescriptorTable table;
		s32				slot;
	};

}// namespcae ob
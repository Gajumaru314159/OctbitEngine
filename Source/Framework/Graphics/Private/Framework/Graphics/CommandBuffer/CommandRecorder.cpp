//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#include <Framework/Graphics/Command/CommandRecorder.h>

namespace ob::graphics
{

	CommandRecorder::CommandRecorder(CommandStorage& storage)
		: m_storage(storage)
	{
		m_storage.increment();
	}

	CommandRecorder::~CommandRecorder() {
		m_storage.merge(m_commands);
	}

}
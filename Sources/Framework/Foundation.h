//***********************************************************
//! @file
//! @brief		フレームワーク基盤
//! @author		Gajumaru
//***********************************************************
#pragma once

#include <Framework/Foundation/PCH/stdafx.h>
#include <Framework/Foundation/Exception/Exception.h>
#include <Framework/Foundation/Log/Assertion.h>
#include <Framework/Foundation/Log/LogMacro.h>
#include <Framework/Foundation/Memory/Allocator/Allocator.h>
#include <Framework/Foundation/Memory/Allocator/MallocAllocator.h>
#include <Framework/Foundation/Memory/Operator/NewOpetrator.h>
#include <Framework/Foundation/Memory/Reference/RefObject.h>
#include <Framework/Foundation/Memory/Reference/Ref.h>
#include <Framework/Foundation/Memory/System/MemorySystem.h>
#include <Framework/Foundation/Memory/Utility/MemoryUtility.h>
#include <Framework/Foundation/Template/Container/BitFlags.h>
#include <Framework/Foundation/Template/Container/HandleForwardList.h>
#include <Framework/Foundation/Template/Container/HandleList.h>
#include <Framework/Foundation/Template/Delegate/Delegate.h>
#include <Framework/Foundation/Template/Event/EventNotifier.h>
#include <Framework/Foundation/Template/Handle/HandleManager.h>

namespace ob{
    using namespace foundation;
}
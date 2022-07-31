//***********************************************************
//! @file
//! @brief  コア
//! @author Gajumaru
//***********************************************************
#pragma once

#include <Framework/Core/Exception/Exception.h>
#include <Framework/Core/Memory/Allocator/Allocator.h>
#include <Framework/Core/Memory/Allocator/MallocAllocator.h>
#include <Framework/Core/Memory/Operator/NewOpetrator.h>
#include <Framework/Core/Memory/System/MemorySystem.h>
#include <Framework/Core/Memory/Utility/MemoryUtility.h>
#include <Framework/Core/Template/Container/BitFlags.h>
#include <Framework/Core/Template/Container/HandleForwardList.h>
#include <Framework/Core/Template/Container/HandleList.h>
#include <Framework/Core/Template/Delegate/Delegate.h>
#include <Framework/Core/Template/Event/EventNotifier.h>
#include <Framework/Core/Template/Handle/HandleManager.h>

#include <Framework/Core/Log/Assertion.h>
#include <Framework/Core/Log/LogMacro.h>

#include <Framework/Core/Allocator/Heap.h>
#include <Framework/Core/Graphic/Color.h>
#include <Framework/Core/Graphic/IntColor.h>

#include <Framework/Core/Geometory/Point.h>
#include <Framework/Core/Geometory/Size.h>
#include <Framework/Core/Geometory/IntRect.h>

#include <Framework/Core/Geometory/Box.h>
#include <Framework/Core/Geometory/Capsule.h>
#include <Framework/Core/Geometory/Circle.h>
#include <Framework/Core/Geometory/Cylinder.h>
#include <Framework/Core/Geometory/Frustum.h>
#include <Framework/Core/Geometory/Margin.h>
#include <Framework/Core/Geometory/Plane.h>
#include <Framework/Core/Geometory/Line.h>
#include <Framework/Core/Geometory/HalfLine.h>
#include <Framework/Core/Geometory/Segment.h>
#include <Framework/Core/Geometory/Rect.h>
#include <Framework/Core/Geometory/Sphere.h>
#include <Framework/Core/Geometory/Triangle.h>

#include <Framework/Core/Math/Vectors.h>
#include <Framework/Core/Math/Affine.h>
#include <Framework/Core/Math/Axis.h>
#include <Framework/Core/Math/FloatCompression.h>
#include <Framework/Core/Math/Math.h>
#include <Framework/Core/Math/Matrix.h>
#include <Framework/Core/Math/Quaternion.h>
#include <Framework/Core/Math/Random.h>
#include <Framework/Core/Math/Rotation.h>
#include <Framework/Core/Misc/UUID.h>
#include <Framework/Core/Math/Random.h>
#include <Framework/Core/Math/Range.h>

#include <Framework/Core/Misc/CRCHash.h>
#include <Framework/Core/Misc/UUID.h>

namespace ob{
    using namespace core;
}
//***********************************************************
//! @file
//! @brief  コア
//! @author Gajumaru
//***********************************************************
#pragma once

#include <Framework/Core/Exception/Exception.h>

#include <Framework/Core/File/BinaryReader.h>
#include <Framework/Core/File/BinaryWriter.h>
#include <Framework/Core/File/BlobStream.h>
#include <Framework/Core/File/FileStream.h>
#include <Framework/Core/File/Path.h>
#include <Framework/Core/File/Stream.h>

#include <Framework/Core/Geometory/Box.h>
#include <Framework/Core/Geometory/Capsule.h>
#include <Framework/Core/Geometory/Circle.h>
#include <Framework/Core/Geometory/Cylinder.h>
#include <Framework/Core/Geometory/Frustum.h>
#include <Framework/Core/Geometory/HalfLine.h>
#include <Framework/Core/Geometory/IntRect.h>
#include <Framework/Core/Geometory/Line.h>
#include <Framework/Core/Geometory/Margin.h>
#include <Framework/Core/Geometory/Plane.h>
#include <Framework/Core/Geometory/Point.h>
#include <Framework/Core/Geometory/Rect.h>
#include <Framework/Core/Geometory/Segment.h>
#include <Framework/Core/Geometory/Size.h>
#include <Framework/Core/Geometory/Sphere.h>
#include <Framework/Core/Geometory/Spline.h>
#include <Framework/Core/Geometory/Spline2D.h>
#include <Framework/Core/Geometory/Triangle.h>
#include <Framework/Core/Geometory/Viewport.h>

#include <Framework/Core/Graphic/Color.h>
#include <Framework/Core/Graphic/HSV.h>
#include <Framework/Core/Graphic/IntColor.h>

#include <Framework/Core/Log/Assertion.h>
#include <Framework/Core/Log/LogMacro.h>

#include <Framework/Core/Math/Axis.h>
#include <Framework/Core/Math/BitOp.h>
#include <Framework/Core/Math/Easing.h>
#include <Framework/Core/Math/FloatCompression.h>
#include <Framework/Core/Math/Vectors.h>
#include <Framework/Core/Math/Math.h>
#include <Framework/Core/Math/Matrix.h>
#include <Framework/Core/Math/Quaternion.h>
#include <Framework/Core/Math/Random.h>
#include <Framework/Core/Math/Range.h>
#include <Framework/Core/Math/Rotation.h>
#include <Framework/Core/Math/Transform.h>

#include <Framework/Core/Memory/Utility/MemoryUtility.h>

#include <Framework/Core/Misc/Blob.h>
#include <Framework/Core/Misc/BlobView.h>
#include <Framework/Core/Misc/Compression.h>
#include <Framework/Core/Misc/CRCHash.h>
#include <Framework/Core/Misc/DateTime.h>
#include <Framework/Core/Misc/TimeSpan.h>
#include <Framework/Core/Misc/Noise.h>
#include <Framework/Core/Misc/UUID.h>
#include <Framework/Core/Misc/YesNo.h>

#include <Framework/Core/Reflection/TypeId.h>

#include <Framework/Core/String/Format.h>
#include <Framework/Core/String/String.h>
#include <Framework/Core/String/StringEncoder.h>

#include <Framework/Core/Template/Container/BitFlags.h>
#include <Framework/Core/Template/Container/HandleForwardList.h>
#include <Framework/Core/Template/Container/HandleList.h>
#include <Framework/Core/Template/Delegate/Delegate.h>
#include <Framework/Core/Template/Event/EventNotifier.h>
#include <Framework/Core/Template/Handle/HandleManager.h>

#include <Framework/Core/Thread/Atomic.h>
#include <Framework/Core/Thread/Mutex.h>
#include <Framework/Core/Thread/ScopeLock.h>
#include <Framework/Core/Thread/SpinLock.h>
#include <Framework/Core/Thread/Thread.h>

#include <Framework/Core/Utility/ConstValue.h>
#include <Framework/Core/Utility/Indexed.h>
#include <Framework/Core/Utility/MemoryStorage.h>
#include <Framework/Core/Utility/Noncopyable.h>
#include <Framework/Core/Utility/Nonmovable.h>
#include <Framework/Core/Utility/Pimpl.h>
#include <Framework/Core/Utility/Scope.h>
#include <Framework/Core/Utility/Ref.h>

namespace ob{
    using namespace core;
}
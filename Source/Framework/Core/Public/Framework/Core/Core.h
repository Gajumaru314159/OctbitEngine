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

#include <Framework/Core/Geometry/Box.h>
#include <Framework/Core/Geometry/Capsule.h>
#include <Framework/Core/Geometry/Circle.h>
#include <Framework/Core/Geometry/Cylinder.h>
#include <Framework/Core/Geometry/Frustum.h>
#include <Framework/Core/Geometry/HalfLine.h>
#include <Framework/Core/Geometry/IntRect.h>
#include <Framework/Core/Geometry/Line.h>
#include <Framework/Core/Geometry/Margin.h>
#include <Framework/Core/Geometry/Plane.h>
#include <Framework/Core/Geometry/Point.h>
#include <Framework/Core/Geometry/Rect.h>
#include <Framework/Core/Geometry/Segment.h>
#include <Framework/Core/Geometry/Size.h>
#include <Framework/Core/Geometry/Sphere.h>
#include <Framework/Core/Geometry/Spline.h>
#include <Framework/Core/Geometry/Spline2D.h>
#include <Framework/Core/Geometry/Triangle.h>
#include <Framework/Core/Geometry/Viewport.h>

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

#include <Framework/Core/String/Format.h>
#include <Framework/Core/String/String.h>
#include <Framework/Core/String/StringEncoder.h>

#include <Framework/Core/Template/Container/BitFlags.h>
#include <Framework/Core/Template/Container/HandleForwardList.h>
#include <Framework/Core/Template/Container/HandleList.h>
#include <Framework/Core/Template/Delegate/Delegate.h>
#include <Framework/Core/Template/Event/EventNotifier.h>

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
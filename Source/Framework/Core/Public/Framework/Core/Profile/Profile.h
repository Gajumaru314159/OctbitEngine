//***********************************************************
//! @file
//! @author		Gajumaru
//***********************************************************
#pragma once

// 複数のプロファイラに対応するためのラッパー
// * Tracy
// * PIX
// * RenderDoc
#include <tracy/Tracy.hpp>

#define OB_PROFILE_THREAD(name)                         tracy::SetThreadNameWithHint(name,-1)

#define OB_PROFILE_SCOPE(name,color)                    ZoneNamedNC(__scope,name,color,true)

#define OB_PROFILE_FRAME()                              FrameMark
#define OB_PROFILE_FRAME_IMAGE(image,w,h,offset,flip)   FrameImage(image,w,h,offset,flip)

#define OB_PROFILE_MESSAGE(message)                     TracyMessageL(message)
#define OB_PROFILE_PLOT(name,value)                     TracyPlot(name,value)

//#define OB_PROFILE_LOCK(type,var)                       TracyLockable(type,var)
#define OB_PROFILE_LOCK(type,var)                       type var
#define OB_PROFILE_LOCK_DESC(type,var,desc)             TracyLockableN(type,var,desc)
#define OB_PROFILE_SHARED_LOCK(type,var)                TracySharedLockable(type,var)
#define OB_PROFILE_SHARED_LOCK_DESC(type,var,desc)      TracySharedLockableN(type,var,desc)

#define OB_PROFILE_ALLOC(category,ptr,size)             TracyAllocS(ptr,size,8)
#define OB_PROFILE_FREE(category,ptr)                   TracyFreeS(ptr,8)

namespace ob::core {

    void BeginProfile();
    void EndProfile();

}
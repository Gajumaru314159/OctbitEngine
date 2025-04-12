//***********************************************************
//! @file
//! @brief		ファイル説明
//! @author		Gajumaru
//***********************************************************
#pragma once

namespace ob::rhi::vulkan {

    inline bool Failed(::VkResult res)
    {
        if (res < 0)
        {
            LOG_ERROR("Vulkanエラー{}",static_cast<s32>(res));
            return true;
        }
        return false;
    }

    inline bool Success(::VkResult res)
    {
        if (res < 0)
        {
            LOG_ERROR("Vulkanエラー{}", static_cast<s32>(res));
            return false;
        }
        return true;
    }


    inline ::VkResult ThrowIfFailed(::VkResult res)
    {
        if (res < 0)
        {
            throw std::runtime_error("Error Vulkan");
        }
        return res;
    }

    //@―---------------------------------------------------------------------------
    //! @brief  説明
    //@―---------------------------------------------------------------------------
    class Utility {
    public:

		template<typename T>
        static void SetName(vk::raii::Device& device,const T& object, StringView name) {
			//vk::DebugReportObjectTypeEXT type = debugReportObjectType(object.objectType);
            //vk::DebugMarkerObjectNameInfoEXT info(type, (uint64_t)((typename T::CType)*object), name.data());
            //device.debugMarkerSetObjectNameEXT(info);
        }

    };






    //===============================================================
    // インライン関数
    //===============================================================
    //! @cond



    //! @endcond
}
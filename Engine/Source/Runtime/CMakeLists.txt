# Library
add_subdirectory ("Core")
add_subdirectory ("Platform")
add_subdirectory ("Input")
add_subdirectory ("RHI")
add_subdirectory ("Graphics")
add_subdirectory ("ImGui")
add_subdirectory ("Model")
#add_subdirectory ("Asset")
add_subdirectory ("Debug")
add_subdirectory ("Engine")
add_subdirectory ("Asset")
add_subdirectory ("Editor")
add_subdirectory ("Runtime")

if(OCTBIT_DIRECTX AND WIN32)
    add_subdirectory ("DirectX12RHI")
endif()

if(OCTBIT_VULKAN)
    add_subdirectory ("VulkanRHI")
endif()

# Executable
add_subdirectory ("Sandbox")
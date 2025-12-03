project (VulkanRHI)

# 追加の依存ライブラリ
set(public_libs 
    Core
    RHI
)
set(private_libs)
if(WIN32)
    list(APPEND private_libs
        "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib"
        "${OCTBIT_THIRD_PARTY_PATH}/dxc/Windows/lib/x64/dxcompiler.lib"
        "${OCTBIT_THIRD_PARTY_PATH}/dxc/Windows/lib/x64/dxil.lib"
    )
endif()
if(UNIX)
    find_package(Vulkan REQUIRED)
    list(APPEND private_libs
        Vulkan::Vulkan
        "${OCTBIT_THIRD_PARTY_PATH}/dxc/Linux/lib/libdxcompiler.so"
        "${OCTBIT_THIRD_PARTY_PATH}/dxc/Linux/lib/libdxil.so"
    )
endif()

# インクルードディレクトリ設定
set(public_includes "")
set(private_includes "$ENV{VK_SDK_PATH}/Include" "${OCTBIT_THIRD_PARTY_PATH}/stb/include")
if(WIN32)
    list(APPEND public_includes "${OCTBIT_THIRD_PARTY_PATH}/dxc/Windows/inc")
endif()
if(UNIX)
    list(APPEND public_includes "${OCTBIT_THIRD_PARTY_PATH}/dxc/Linux/include/dxc")
endif()

OB_ADD_LIBRARY()
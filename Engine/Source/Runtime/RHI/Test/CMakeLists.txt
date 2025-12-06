project (RHI-test)

# 依存ライブラリ設定
set(public_libs RHI)
set(private_libs "")

if(OCTBIT_DIRECTX AND WIN32)
    list(APPEND private_libs DirectX12RHI)
endif()

if(OCTBIT_VULKAN)
    list(APPEND private_libs VulkanRHI)
endif()

# インクルードディレクトリ設定
set(public_includes "")
set(private_includes "")

OB_ADD_TEST()
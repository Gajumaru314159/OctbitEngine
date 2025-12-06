project (Debug)

# インクルードディレクトリ設定
set(public_includes "")
set(private_includes "")

# 依存ライブラリ設定
set(public_libs  Core Platform Graphics Engine Input ImGui)
set(private_libs "")
if(WIN32)
    list(APPEND private_libs dinput8.lib dxguid.lib)
endif()

OB_ADD_LIBRARY() 

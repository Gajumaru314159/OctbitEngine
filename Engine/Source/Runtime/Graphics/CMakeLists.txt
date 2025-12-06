project (Graphics)

# インクルードディレクトリ設定
set(public_includes "")
set(private_includes "")

# 依存ライブラリ設定
set(public_libs RHI ImGui FrameGraph)
set(private_libs assimp)

OB_ADD_LIBRARY()

ob_add_test_module()
project (Model)

# インクルードディレクトリ設定
set(public_includes "")
set(private_includes "")

# 依存ライブラリ設定
set(public_libs Core Platform Graphics assimp)
set(private_libs "")

OB_ADD_LIBRARY()
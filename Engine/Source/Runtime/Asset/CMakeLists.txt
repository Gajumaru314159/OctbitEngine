project (Asset)

# インクルードディレクトリ設定
set(public_includes "")
set(private_includes "")

# 依存ライブラリ設定
set(public_libs Core)
set(private_libs "")

OB_ADD_LIBRARY()

ob_add_test_module()
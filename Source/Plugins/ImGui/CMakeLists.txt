cmake_minimum_required (VERSION 3.15)
project (ImGui)

# インクルードディレクトリ設定
set(public_includes "")
set(private_includes "${CMAKE_CURRENT_SOURCE_DIR}/Public/Plugins/ImGui/Library/misc/freetype" "${CMAKE_CURRENT_SOURCE_DIR}/Public/Plugins/ImGui/Library")

# 依存ライブラリ設定
set(public_libs Core Platform RHI Input)
set(private_libs "")

OB_ADD_PLUGIN("${public_includes}" "${private_includes}" "${public_libs}" "${private_libs}") 
cmake_minimum_required (VERSION 3.15)
project (Graphics)

# インクルードディレクトリ設定
set(public_includes "")
set(private_includes "")

# 依存ライブラリ設定
set(public_libs Core RHI ImGui FrameGraph)
set(private_libs assimp)
if(WIN32)
    list(APPEND private_libs DirectX12RHI)
endif()

OB_ADD_FRAMEWORK_LIBRARY("${public_includes}" "${private_includes}" "${public_libs}" "${private_libs}")

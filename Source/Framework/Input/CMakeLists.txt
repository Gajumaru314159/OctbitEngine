cmake_minimum_required (VERSION 3.15)
project (Input)

# インクルードディレクトリ設定
set(public_includes "")
set(private_includes "")

# 依存ライブラリ設定
set(public_libs Core Platform)
set(private_libs dinput8.lib dxguid.lib)

OB_ADD_FRAMEWORK_LIBRARY("${public_includes}" "${private_includes}" "${public_libs}" "${private_libs}")
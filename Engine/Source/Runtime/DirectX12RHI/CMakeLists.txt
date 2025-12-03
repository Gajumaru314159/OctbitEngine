project (DirectX12RHI)

# 依存ライブラリ設定
set(public_libs 
    Core
    RHI
)
set(private_libs 
    DirectXTex
    d3d12.lib
    dxgi.lib
    dxguid.lib
    "${OCTBIT_THIRD_PARTY_PATH}/PIX/bin/WinPixEventRuntime.lib"
    "${OCTBIT_THIRD_PARTY_PATH}/dxc/Windows/lib/x64/dxcompiler.lib"
    "${OCTBIT_THIRD_PARTY_PATH}/dxc/Windows/lib/x64/dxil.lib"
    "${OCTBIT_THIRD_PARTY_PATH}/DirectStorage/native/lib/x64/dstorage.lib"
)

# インクルードディレクトリ設定
set(public_includes "")
set(public_includes
    "${OCTBIT_THIRD_PARTY_PATH}/PIX/include"
    "${OCTBIT_THIRD_PARTY_PATH}/Windows/dxc/inc"
    "${OCTBIT_THIRD_PARTY_PATH}/DirectStorage/native/include"
)
set(private_includes "")

OB_ADD_LIBRARY()
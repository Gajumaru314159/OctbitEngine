# IndirectDraw {#IndirectDeraw}

## 置き換え可能な機能
* draw
* drawIndexed
* dispatch
* setRootConstant
* setVertexBuffer
* setVertexBuffers
* setIndexBuffer

> [!NOTE]
> DirectX12的にはRootDescriptorの設定も可能ですが、OctbitEngineではDescriptorTableに抽象化しているため非対応です。

# プラットフォームの差異の吸収

|OctbitEngine|DirectX12|Vulkan|
|-|-|-|
|-|[D3D12_DRAW_ARGUMENTS](https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ns-d3d12-d3d12_draw_arguments)|[VkDrawIndirectCommand](https://registry.khronos.org/vulkan/specs/latest/man/html/VkDrawIndirectCommand.html)|
|-|[D3D12_DRAW_INDEXED_ARGUMENTS](https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ns-d3d12-d3d12_draw_indexed_arguments)|[VkDrawIndexedIndirectCommand](https://registry.khronos.org/vulkan/specs/latest/man/html/VkDrawIndexedIndirectCommand.html)|
|-|[D3D12_DISPATCH_ARGUMENTS](https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ns-d3d12-d3d12_dispatch_arguments)|[VkDispatchIndirectCommand](https://registry.khronos.org/vulkan/specs/latest/man/html/VkDispatchIndirectCommand.html)|
|-|[D3D12_VERTEX_BUFFER_VIEW](https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ns-d3d12-d3d12_vertex_buffer_view)|[VkBuffer](https://registry.khronos.org/vulkan/specs/latest/man/html/VkBuffer.html) + [VkDeviceMemory](https://registry.khronos.org/vulkan/specs/latest/man/html/VkDeviceMemory.html)|
|-|[D3D12_INDEX_BUFFER_VIEW](https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ns-d3d12-d3d12_index_buffer_view)|[VkBuffer](https://registry.khronos.org/vulkan/specs/latest/man/html/VkBuffer.html) + [VkDeviceMemory](https://registry.khronos.org/vulkan/specs/latest/man/html/VkDeviceMemory.html)|

VulkanはD3D12_VERTEX_BUFFER_VIEW/D3D12_INDEX_BUFFER_VIEWに相当する構造体はありません。1つの巨大なバッファをバインドしてオフセットを指定して描画する必要があります。
[VK_NV_device_generated_commands](https://registry.khronos.org/vulkan/specs/latest/man/html/VK_NV_device_generated_commands.html)拡張を使用すると

# 参考
* [Direct3D 12 間接的な描画](https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/indirect-drawing#indirect-argument-buffer-structures)
* [Vulkan Guide Draw Indirect API](https://vkguide.dev/docs/gpudriven/draw_indirect/)
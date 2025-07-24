# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

This is a CMake-based C++ project. Use these commands to build and test:

```bash
# Generate build files and build the project
cmake -B Build/x64-Debug -S . -A x64
cmake --build Build/x64-Debug --config Debug

# Run unit tests
cd Build/x64-Debug
ctest -C Debug

# Build specific modules
cmake --build Build/x64-Debug --config Debug --target Core
cmake --build Build/x64-Debug --config Debug --target RHI
cmake --build Build/x64-Debug --config Debug --target Graphics
```

## Project Structure

OctbitEngine is a multi-platform game engine written in C++20 with a modular architecture:

### Framework Modules (Source/Framework/)
- **Core** - Foundation utilities, math, DI container, reflection system, memory management
- **Platform** - Window management, file I/O, platform abstraction
- **RHI** - Rendering Hardware Interface abstracting DirectX12/Vulkan
- **Graphics** - High-level rendering systems built on RHI, FrameGraph-based pipeline
- **Engine** - ECS (Entity Component System) with archetype-based organization
- **Input** - Input device management and abstraction
- **Asset** - Asset loading and management system
- **Debug** - Debugging tools and profiler
- **Editor** - Editor-specific functionality

### Plugin System (Source/Plugins/)
- **DirectX12RHI** - DirectX 12 implementation
- **VulkanRHI** - Vulkan implementation  
- **ImGui** - Immediate mode GUI integration

### Testing (Source/Test/)
Unit tests for each framework module using GoogleTest.

## Key Architecture Patterns

### Dependency Injection
The engine uses a central DI system with `ServiceInjector` and `ServiceContainer`. Services are registered via `RegisterService<Interface, Implementation>()` and resolved automatically.

### Entity Component System (ECS)
Modern ECS with archetype-based storage for cache-friendly performance. Components are managed in chunks for efficient bulk operations.

### FrameGraph Rendering
Declarative rendering pipeline that automatically manages GPU resources and optimizes execution order.

### RHI Abstraction
Common interface for DirectX12 and Vulkan with unified resource types (`Buffer`, `Texture`, `CommandList`, `PipelineState`).

## Development Notes

- Uses C++20 standard with modern patterns (smart pointers, RAII)
- All modules use `ob::` namespace (with using declarations for convenience)
- Smart pointer types: `Ref<>` for shared ownership, `UPtr<>` for unique ownership
- Plugin selection happens at runtime based on configuration
- Editor is built as separate C# WPF application communicating via network protocol

## Testing

Run tests for specific modules:
```bash
# From Build/x64-Debug
ctest -C Debug -R Core-test
ctest -C Debug -R RHI-test
ctest -C Debug -R Graphics-test
```

## Third-Party Dependencies

Uses FetchContent for dependency management including:
- GSL (Microsoft Guidelines Support Library)
- GoogleTest for unit testing
- FrameGraph for rendering pipeline
- Assimp for model loading
- DirectXTex (Windows only)
- Vulkan SDK libraries (when Vulkan enabled)
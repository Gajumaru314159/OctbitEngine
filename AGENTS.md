# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

This is a CMake-based C++ project. Use these commands to build and test:

Windows

```bash
# From Root Directory

# Generate build files
# if you add or remove files, you must run this command again.
cmd.exe /c Build.bat setup

# Build the project
cmd.exe /c Build.bat

# Build specific modules
# When building the Core module, you can build UnitTest at the same time by building Core-test.
cmd.exe /c Build.bat RHI-test
```

Linux(WSL)

```bash

# From Root Directory

# Generate build files
# if you add or remove files, you must run this command again.
/usr/bin/cmake --build -G "Ninja" -B Build/WSL

# Build the project
/usr/bin/cmake --build Build/WSL -j 22

# Build specific modules
# When building the Core module, you can build UnitTest at the same time by building Core-test.
/usr/bin/cmake --build Build/WSL --target Core-test -j 22

```

## Project Structure

OctbitEngine is a multi-platform game engine written in C++20 with a modular architecture:

### Framework Modules (Engine/Source/Framework/)

- **Core** - Foundation utilities, math, DI container, reflection system, memory management
- **Platform** - Window management, file I/O, platform abstraction
- **RHI** - Rendering Hardware Interface abstracting DirectX12/Vulkan
- **Graphics** - High-level rendering systems built on RHI, FrameGraph-based pipeline
- **Engine** - ECS (Entity Component System) with archetype-based organization
- **Input** - Input device management and abstraction
- **Asset** - Asset loading and management system
- **Debug** - Debugging tools and profiler
- **Editor** - Editor-specific functionality
- **DirectX12RHI** - DirectX 12 implementation
- **VulkanRHI** - Vulkan implementation
- **ImGui** - Immediate mode GUI integration

## Key Architecture Patterns

### Dependency Injection

The engine uses a central DI system with `ServiceInjector` and `ServiceContainer`. Services are registered via `RegisterService<Interface, Implementation>()` and resolved automatically.

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
# From Root Directory
Build/Bin/Core-test.exe
Build/Bin/RHI-test.exe
Build/Bin/Platform-test.exe
Build/Bin/Graphics-test.exe
```

## Third-Party Dependencies

Uses FetchContent for dependency management including:

- GSL (Microsoft Guidelines Support Library)
- GoogleTest for unit testing
- FrameGraph for rendering pipeline
- Assimp for model loading
- DirectXTex (Windows only)
- Vulkan SDK libraries (when Vulkan enabled)

## IMPORTANT

- Before completing the task, be sure to build and test to ensure it works properly.
- Please explain the work and have conversations in Japanese.

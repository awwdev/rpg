# 3D FRAMEWORK
C++ 3D framework / engine with custom "STL" (utilities).
Little use of third party tools (besides vulkan.h).

**Intended to show my experience with programming and software engineering.**
It is not intended to be a "plug in and play" library nor is it an universal 3D engine.
It might get the base for some stylized low poly game in the future?

![](media/progress/2020_09_03_grassPerf.gif)
![](media/progress/2020_08_25_grass.gif)
![](media/progress/2020_08_14_terrainEdit.gif)
![](media/progress/2020_08_04_terrainStichPlayerMove.gif)
![](media/progress/2020_07_04_ui_wnd.gif)

### Contents
- Custom "STL" (Array, String, Bitset, Maps)
- Memory allocation (some CTFE)
- Logger and profiler (HTML memory print out)
- Math and algorithms (Matrix, Quaternion)
- Window and input system
- Vulkan renderer (3D, terrain, player controller, shadows) and immediate GUI
- Other utils

**This is Work In Progress!**


## How to run?
- I am using VS Code with some C++ extensions
- Batch files are included to fire up the compiler MSVC
- Need of C++20 (heavy use of "designated initializers" for Vulkan)


# Rationale

## Custom STL
The std is hard to read and hard to extend (e.g. using std::vector with a custom allocator).
So I decided to roll my own containers and memory allocation.

![](media/progress/2020_08_08_AllocationView.png)

## Entity-Component-System
- DOD (SOA)
- Custom parsing from txt file (prefabs)

## Vulkan
- Context   (Instance, PhysicalDevice, LogicalDevice, Meta, Debug)
- Resources (Pipeline, Shader, Renderpass, Synchronization, ...)
- Rendering (Command recording, presentation loop)
- Custom parsing of mesh data (custom blender export)

Files in third/ directory are licensed separately, please view these files for more information.

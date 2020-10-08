# 3D FRAMEWORK
C++ 3D framework / engine with custom "STL" (utilities).
Little use of ext party tools (besides vulkan.h).

**Intended to show my experience with programming and software engineering.**
It is not intended to be a "plug in and play" library nor is it an universal 3D engine.
It might become the base for a stylized low poly game in the future ...


![](ads/progress/2020_10_06_bloomVoronoi.gif)
![](ads/progress/2020_09_03_grassPerf.gif)
![](ads/progress/2020_10_08_sceneTree.gif)
![](ads/progress/2020_08_14_terrainEdit.gif)
![](ads/progress/2020_08_04_terrainStichPlayerMove.gif)
![](ads/progress/2020_07_04_ui_wnd.gif)
![](ads/progress/2020_09_18_blur2.gif)
![](ads/progress/2020_08_08_AllocationView.png)

### Contents
- Custom "STL" (Array, String, Bitset, Maps)
- mem allocation (some CTFE)
- Logger and profiler (HTML memory print out)
- Math and algorithms (Matrix, Quaternion)
- Window and input system
- Vulkan renderer (3D, terrain, player controller, shadows) and immediate GUI
- Other utils

**This is Work In progress!**

## How to run?
- I am using VS Code with some C++ extensions
- Batch files are included to fire up the compiler (mingw GCC)
- Need of C++20 (heavy use of "designated initializers" for Vulkan)

# Rationale

## Code Style
This codebase does not follow the classy OOP paradigm (inheritance, encapsulation).
Personally, I found a straightforward and procedural approach more satisfying.

## Custom STL
The std is hard to read and hard to extend (e.g. using std::vector with a custom allocator).
So I decided to roll my own containers and memory allocation.

## Entity-Component-System
- DOD (SOA)
- Custom parsing from txt file (prefabs)

## Vulkan
- Context  (Instance, PhysicalDevice, LogicalDevice, Swapchain)
- Objects  (Buffer, Image, Descriptors)
- States   ("dedicated structs" per state)
- Renderer (Presentation, synoptic)

- Custom parsing of mesh data (custom blender export)

Files in ext/ directory are licensed separately, please view these files for more information.
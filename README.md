# Mini 2020
This is the base code for my ongoing **game / multimedia project**.
It is intended to **show my experience with programming** in C++ and software engineering.
It can also be used as framework and reference for your own engine.
I choose the name "mini" since I like to write elegant and minimal code (as much as possible).

![](media/progress/2020_07_04_ui_wnd.gif)
![](media/progress/2020_01_23_grassMoving.gif)
Second image is from my old Vulkan renderer (3D is WIP).

### Contents
- Containers (Array, Bitset, String, Map)
- Global allocator
- Logger and Profiler
- Math (Matrix, Quaternion)
- Window and event system
- Vulkan Renderer and UI
- Other utils

This is Work In Progress!


## How to run?
- I am using VS Code (with some C++ extensions) 
- Batch files are included to fire up the compiler (msvc)
- Need of C++20 (heavy use of "designated initializers" for vulkan)
- You probably need to spend some time to setup your IDE / editor to run this code, but everything is included


# Rationale

## mini::box::Array
- fixed size (capacity-based with internal count)
	- auto growth can be harmful on perf (reallocations), can lead to ptr invalidation and makes it harder to use with custom allocators
	- simple inheritance (without vtable) is used so the (abstract) base can be passed around without the need of writing Array<T, N> everywhere
	- wrapper and elements are not seperated in memory and the whole object can simply be passed to an allocator
	- AppendArray() can be used to resize or convert an array 
- no ctor calls when initializing the array (underlying byte array)
- array is usable with enums (no cast by the user needed)
- bounds checking is toggleable via macro, no exceptions are used
- removal of elements can be done fast (swapping) or slow while preserving order
- avoidance of cpy/mv ctor and assignment boilerplate - instead use generic AppendArray
- overall more readable and extendible than the STL

## mini::box::Bitset
- bitset is usable with enums (no cast by the user needed)
- bounds checking is toggleable via macro, no exceptions are used
- some constexpr methods and inlined (templated) methods (like Set<0, true>())
- overall more readable and extendible than the STL

## mini::box::String
- fixed size (capacity-based with internal count)
	- auto growth can be harmful on perf (reallocations) and makes it harder to use with custom allocators
	- simple inheritance (without vtable) is used so the (abstract) base can be passed around without the need of writing String<N> everywhere
	- wrapper and elements are not seperated in memory and the whole object can simply be passed to an allocator
- overloads for array, ptrs and the class 
- bounds checking is toggleable via macro, no exceptions are used
- overall more readable and extendible than the STL

## mini::box::Map
- no bucket hash system, instead intended to be used with plain enums and integers
- fixed size (capacity-based with internal count)
- no ctor calls when initializing the array (underlying byte array)
- bounds checking is toggleable via macro, no exceptions are used
- using a bitset to check if key exists (unique keys)

## mini::mem::Allocate()
- global functions
- compile time (capacity based system)
    - user defines blocks at compile time in one place (see array ALLOC_INFOS[])
    - when claiming memory, the appropriate block size will be figured out at compile time
- BlockPtr
    - is returned when claiming memory
    - holds data about the used block and will "free" the block when destroyed (RAII)
- free / used blocks are represented by one bitset
- printable as HTML

## mini::wnd
- straight forward winapi
- event abstractions (event contains union)
- bitset holds state of input (pressed keys and buttons)

## mini::vk
- Context   (Instance, PhysicalDevice, LogicalDevice, Meta, Debug)
- Resources (Pipeline, Shader, Renderpass, Synchronization)
- Rendering (Command recording, presentation loop)
- Objects   (UBO, VBO, Pipeline, Shader, RenderPass, that come with CreateX_Y methods where X is the obj and Y something like UI)

## mini::ui
- wip (immediate gui)

...wip
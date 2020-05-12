# MiniSTL
Minimal basic library - simple, readable and extendable.

- Containers (Array, Bitset, String)
- Global allocator functions
- Logger and Profiler
- Math (Matrix, Quaternion)
- Window and window events
- Vulkan Renderer
- Misc (Types, ...)

This is Work In Progress!


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

## mini::mem alloc functions
- global functions, simple usage
- compile time (capacity based system)
    - user defines blocks at compile time in one place (see array allocs[])
    - when claiming memory, the appropriate block size will be figured out at compile time
- MemPtr
    - is returned when claiming memory
    - holds data about the used block and will "free" the block when destroyed (RAII)
- free / used blocks are represented by one bitset
- printable

## mini::wnd
- straight forward winapi
- event abstractions (event contains union)
- bitset holds state of input (pressed keys and buttons)


...wip
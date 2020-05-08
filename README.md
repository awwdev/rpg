# MiniSTL
Minimal basic library - simple, readable and extendable.

- Containers (Array, Bitset, String, Variant, Optional?)
- Allocator
- Logger
- Math (Matrix, Quaternion)
- Profiler
- Window and window events
- Renderer
- Misc (Types)

This is Work In Progress!

# Rationale

## mini::Array
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

## mini::Bitset
- some constexpr methods
- no proxies (no use of operator[])
- should be more readable, debuggable and extendable than the STL
- no exceptions - bound checks can be toggle via macro

## mini::Allocator
- global and pure functions
- block pools (defined at compile time)
- no use of new or malloc (possible os calls) and overhead by malloc (which may increase by long term fragmentation)

## mini::String

## mini::Variant

## mini::Optional (tbd)

## mini::Logger (tbd)
- can be toggle via macro (no print)
- store logs in buffer or external

## mini::Profiler
- based on RAII (object lifetime, ctor and dtor call now())

## mini::Window
- os specific events abstracted into os agnostic events
- effective event pipeline

## mini::Renderer

## mini::Math
- vector is also a matrix
- so much more readable than glm

## Misc
- typedefs and concepts (or rather SFINAE)
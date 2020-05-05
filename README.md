# MiniSTL
Minimal basic library - simple, readable and extendable.

- Containers (Array, Bitset, String, Variant)
- Allocator
- Logger
- Math (Matrix, Quaternion)
- Profiler
- Window and window events
- Renderer
- Misc (Types)

This is Work In Progress!


## mini::Array
- fixed size: 
	- auto growth can be harmful on perf, can lead to ptr invalidation bugs and makes it harder to use with allocators
	- array can be copied/moved into other sized arrays with convertible type (may result in loss of data)
	- array can be used the same way on both stack and heap (managed by an allocator)
	- due to inheritance (virtuality not used though) you can use the size agnostic base for passing around (no Array<T, SIZE>& but Array<T>&)
	- objects lives completely on either heap or stack (no jumping around like vector)
	- do not confuse it with std::array which calls ctors and has no concept of capacity (and no erase)
- enums can be passed to the array (no outer cast by the user needed)
- no exceptions - bound checks can be toggle via macro
- array does not assume you want to preserve order, there is Remove (O(1)) and RemoveOrdered (O(n))
- array initialization will not trigger ctors of elements
- should be more readable, debuggable and extendable than the STL
- while I tend not to use much of OOP (I like procedural and DOD), in this specific case accessors (hide "count" for example) and inheritance make perfect sense
      (because under no imaginable circumstances you would want that the count is set from outside, so there is only a "Getter", also "abstract" IArray shall not be instantiated on its own)

## mini::Bitset

## mini::Allocator
- global and pure functions
- block pools (defined at compile time)
- no use of new or malloc (possible os calls)

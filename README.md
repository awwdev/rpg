# MiniSTL
Minimal library for (game) projects. Simple, readable and extendable.

- Containers (Array, Bitset, Map(wip))
- Allocator
- Logger
- Math (Vector, Matrix, Quaternion)
- Profiler
- Misc (Types)


This is Work In Progress!


## mini::Array
- preserving order is optional (fast insert and removal possible)
- no exceptions, checks can be toggled via macro
- fixed size (capacity based, copy into bigger array possible but user must do that)
- segregation in base (IArray) and derived (Array) makes it easier to pass array around (no need to write Array<T, size>), no virtual methods used

## mini::Bitset

## mini::Allocator
- global and pure functions
- block pools (defined at compile time)
- no use of new or malloc (os calls)

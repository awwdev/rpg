#include "MiniSTL/Memory/Allocator.hpp"
#include "MiniSTL/Container/Array.hpp"
#include "MiniSTL/Memory/AllocatorPrint.hpp"
#include "MiniSTL/Debug/Logger.hpp"

struct Foo {
    char arr[127] = "Hello World";
};

void WorkOnFoo(mini::mem::MemOwnerX<Foo>& foo)
{
    std::cout << foo->arr << "\n";
}

///TEST
int main()
{
    mini::mem::Allocate();

    mini::container::Array<mini::mem::MemOwnerX<Foo>, 20> objs;
    for (auto i = 0; i < objs.CAPACITY; ++i)
    {
        objs.Append(mini::mem::Claim<Foo>());
    }
    


    mini::mem::PrintAllocationHTML();

    WorkOnFoo(objs[0]);

    std::cin.get();
    return 0;
}
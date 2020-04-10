#include "MiniSTL/Array.hpp"
#include "MiniSTL/Allocator.hpp"
#include <iostream>

struct Foo
{
    int* num = nullptr;

    Foo()                              { std::cout << "ctor called\n";         num = new int(42); }
    explicit Foo(const int i)          { std::cout << "ctor(n) called\n";      num = new int(i); }
    Foo(const Foo& other)              { std::cout << "copy ctor called\n";    num = new int(*other.num); }
    void operator=(const Foo& other)   { std::cout << "copy assign called\n";  *num = *other.num; }
    Foo(Foo&& other)                   { std::cout << "move ctor called\n";    num = other.num; other.num = nullptr; }
    void operator=(Foo&& other)        { std::cout << "move assign called\n";  num = other.num; other.num = nullptr; }
    ~Foo()                             { std::cout << "dtor\n"; if (num!=nullptr) delete num; }
};
void PrintIntArray(const char*, const mini::container::IArray<int>&); //can consume any size
void PrintFooArray(const char*, const mini::container::IArray<Foo>&); //can consume any size


///TEST
int main()
{
    mini::mem::Allocate(1'000'000);
    auto ptr = mini::mem::Claim<Foo>(42);
    *(ptr->num) = 42;
    std::cout << *(ptr->num) << '\n';
    mini::mem::Free(ptr);
    
    //mini::profile::AllocateProfileHistory(); //when the user actually wants to use it
    //
    //mini::container::Array<Foo, 128> arr1;
    //for (auto i = 0; i < arr1.CAPACITY; ++i) {
    //    arr1.EmplaceBack(i);
    //}
    //auto arr2 { arr1 };
    //
    //arr1.RemoveOrdered(0);
    //arr1.RemoveOrdered(1);
    //arr1.RemoveOrdered(2);
    //arr1.RemoveOrdered(3);
    //system("CLS");
    //
    //mini::profile::PrintProfileHistory();
    //PrintFooArray("arr1", arr1);

    std::cin.get();
    return 0;
}


void PrintIntArray(const char* name, const mini::container::IArray<int>& arr)
{
    std::cout << "------arr print: " << name << '\n';
    FOR_ARRAY(arr, i) { std::cout << arr[i] << '\n'; }
    std::cout << "------\n";
}

void PrintFooArray(const char* name, const mini::container::IArray<Foo>& arr)
{
    std::cout << "------arr print: " << name << '\n';
    FOR_ARRAY(arr, i) { std::cout << *arr[i].num << '\n'; }
    std::cout << "------\n";
}
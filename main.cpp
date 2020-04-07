#include <iostream>
#include "Array.h"

struct Foo
{
    int num = 42;
    Foo()                                       { std::cout << "ctor called\n"; }
    explicit Foo(const int i) : num { i }       { std::cout << "ctor(n) called\n"; }
    Foo(const Foo& other) : num{ other.num }    { std::cout << "copy ctor called\n"; }
    void operator=(const Foo& other)            { std::cout << "copy assign called\n"; num = other.num; }
    Foo(Foo&& other) : num{ other.num }         { std::cout << "move ctor called\n"; }
    void operator=(Foo&& other)                 { std::cout << "move assign called\n"; num = other.num; }
    ~Foo()                                      { std::cout << "dtor\n"; }
};
void PrintIntArray(const char*, const mini::IArray<int>&); //can consume any size
void PrintFooArray(const char*, const mini::IArray<Foo>&); //can consume any size


///TEST
int main()
{
    mini::Array<int, 4> arr1 { 0, 1, 2 };
    arr1.EmplaceBack(3);
    //arr1.EmplaceBack(4); //assert fails
    PrintIntArray("arr1", arr1);
    arr1.Remove(1);
    std::cout << "arr1 contains 2: " << std::boolalpha << (arr1.Contains(2) != nullptr) << '\n';
    mini::Array<int, 8> arr2 = arr1;
    PrintIntArray("arr2", arr2);
    arr2.Clear();
    PrintIntArray("arr2", arr2);
    
    auto arr3 = new mini::Array<Foo, 1024> { Foo { 42 }, Foo { 123 } };
    PrintFooArray("arr3", *arr3);
    delete arr3;

    std::cin.get();
    return 0;
}


void PrintIntArray(const char* name, const mini::IArray<int>& arr)
{
    std::cout << "------arr print: " << name << '\n';
    FOR_ARRAY(arr, i) { std::cout << arr[i] << '\n'; }
    std::cout << "------\n";
}

void PrintFooArray(const char* name, const mini::IArray<Foo>& arr)
{
    std::cout << "------arr print: " << name << '\n';
    FOR_ARRAY(arr, i) { std::cout << arr[i].num << '\n'; }
    std::cout << "------\n";
}
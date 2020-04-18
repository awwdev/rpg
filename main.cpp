#include "MiniSTL/Memory/Allocator.hpp"
#include "MiniSTL/Container/Array.hpp"
#include "MiniSTL/Memory/AllocatorPrint.hpp"
#include "MiniSTL/Debug/Logger.hpp"

struct Component
{
    std::size_t num1 = 42;
    std::size_t num2 = 333;
    float num3 = 777;
    char desc[100] = "Some description text.";
};

void ProcessComponent(const Component& component)
{
    std::cout << component.desc << "\n";
}

///TEST
int main()
{
    mini::mem::Allocate();

    ///component array
    using ComponentArray = mini::container::Array<Component, 1000>;
    constexpr auto COMPONENT_SIZE = sizeof(Component);
    constexpr auto SIZE = sizeof(ComponentArray);

    auto componentArray = mini::mem::ClaimBlock<ComponentArray>();
    for (auto i = 0; i < componentArray->CAPACITY; ++i)
    {
        componentArray->Append();
    }


    ProcessComponent((*componentArray)[0]);
    mini::mem::PrintAllocationHTML();


    std::cin.get();
    return 0;
}
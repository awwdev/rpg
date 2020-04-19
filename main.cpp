#include "MiniSTL/Memory/Allocator.hpp"
#include "MiniSTL/Memory/AllocatorPrint.hpp"
#include "MiniSTL/Container/Array.hpp"
#include "MiniSTL/Debug/Logger.hpp"

#include "MiniSTL/Math/Vector.hpp"
#include "MiniSTL/Math/Matrix.hpp"

int main()
{
    using namespace mini::math;

    Vec4f vec{ 1, 2, 3, 0 };
    vec *= 2;
    DLOG(vec);

    const auto mat = Identity4<float>();

    auto m2 = mat * vec;
    DLOG(m2);

    std::cin.get();
}









/*

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
    FOR_ARRAY((*componentArray), i)
    {
        ProcessComponent((*componentArray)[i]);
    }
    mini::mem::PrintAllocationHTML();

    std::cout << "done";
    std::cin.get();
    return 0;
}

*/
#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/Math/Matrix.hpp"

int main()
{
    using namespace mini::math;

    auto m1 = Identity4x4<float>();
    auto m2 = Identity4x4<float>();

    m1 = { 0, 1, 2, 3 };

    std::cout << std::boolalpha;
    DLOG(m1 == m2);


    std::cin.get();
}
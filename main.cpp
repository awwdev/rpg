#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/Math/Matrix.hpp"

int main()
{
    using namespace mini::math;

    auto m1 = Identity4x4<int>();
    m1[0][0] = 42;
    auto m2 = Identity4x4<float>();
    m2[0][0] = 0.123f;

    auto m3 = m1 + m2;

    DLOG(m1);
    DLOG(m2);
    DLOG(m3);


    std::cin.get();
}
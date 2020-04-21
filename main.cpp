#include "MiniSTL/Debug/Logger.hpp"
#include "MiniSTL/Math/Matrix.hpp"

int main()
{
    using namespace mini::math;

    Vec2f v1 = { 1, 2 };
    Vec2f v2 = { 2, 3 };
    Vec2f v3 = { 4, 5 };
    Vec2f v4 = v1 + v2;

    DLOG(v4[X0]);

    std::cin.get();
}
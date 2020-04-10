#include "MiniSTL/Allocator.hpp"

namespace mini::mem
{
    std::byte* base = nullptr;
    container::Array<IBlock, 1024> freeBlocks;
    container::Array<IBlock, 1024> usedBlocks;
}
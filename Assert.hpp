#pragma once

namespace mini {
    inline auto Assert(const bool b) { if (!b) { __debugbreak(); } } // vsonly
}
#pragma once
#include "dbg/Logger.hpp"

//TODO: const std::source_location& location = std::source_location::current()
//#include <source_location>

#include <fstream>

namespace rpg::dbg {

constexpr void Assert(bool const expr, const auto& msg) 
{ 
    if (expr == true)
        return;

    if (!std::is_constant_evaluated())
        dbg::LogError("[ASSERTION FAILED]", msg);

    __debugbreak();
} 



}//NS
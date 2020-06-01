#pragma once
#include "mini/Memory/Allocator.hpp"
#include <fstream>

namespace mini::mem
{
    //! TODO WIP
    //basic idea: dont use html table or divs but text chars like a rect

    inline void PrintAllocationHTML()
    {
        std::ofstream out("AllocationView.html");

        out << R"(
<!DOCTYPE html>
<head>
    <title>Allocation View</title>
    <meta charset="utf-8">

    <style>
        * { padding:0; margin:0; border:0; font-size: 12px; font-family: monospace; }
        div { word-break: break-all; width: 400px; height: 100%; font-size: 24px; border: 1px solid gray; }
    </style>
</head>
<body>

<div>
)";

        FOR_BITSET(blocksUsed, i)
        {
            out << (blocksUsed.Test(i) ? "&#9639;" : "&#9634;");
        }

        out << "</div></body>";

    }

}//ns
#pragma once
#include "mini/Memory/Allocator.hpp"
#include <fstream>

namespace mini::mem
{
    inline void PrintAllocationHTML()
    {
        std::ofstream out("AllocationView.html");

        out << R"(
<!DOCTYPE html>
<head>
    <title>Allocation View</title>
    <meta charset="utf-8">

    <style>
        *       { padding:0; margin:0; border:0; font-size: 24px; font-family: monospace; }
        div     { word-break: break-all; width: 400px; height: 100%; border: 1px solid gray; }
        a:hover { cursor: pointer; }
    </style>
</head>
<body>

<div>
)";
/*
        u32 currAllocIdx = 0;
        FOR_BITSET(blocksUsed, i)
        {
            const auto allocIdx = GetAllocIdxFromBlockId(i);
            if (currAllocIdx != allocIdx)
            {
                currAllocIdx = allocIdx;
                out << "<br>";
            }
            auto* blockType = blockTypes.GetValueOptional(i);
            
            out << "<a title='";
            out << (blockType == nullptr ? "unused" : *blockType);
            out << (blocksUsed.Test(i) ? "'>&#9639;</a>" : "'>&#9634;</a>"); //filled and not filled rect
        }

        out << "</div></body>";
*/

    }

}//ns
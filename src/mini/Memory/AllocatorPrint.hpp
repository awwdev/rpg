//https://github.com/awwdev

#pragma once
#include "mini/Memory/Allocator.hpp"
#include <fstream>

namespace mini::mem
{
inline void PrintAllocationHTML()
{
    std::ofstream out("log/AllocationView.html");

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

<div><!-- blocks -->
)";

    idx_t currentArrayIdx = 0;
    out << "<div width=100px>" << BLOCK_ARRAYS[currentArrayIdx].count << " | " << BLOCK_ARRAYS[currentArrayIdx].size << "</div>";
    FOR_BITSET(priv::blocksUsed, i)
    {
        const auto nextArrayIdx = GetBlockArrayIdxFromBlockId(i);
        if (currentArrayIdx != nextArrayIdx) {
            currentArrayIdx = nextArrayIdx;
            out << "<br>";//new line for a new array
            out << "<div width=100px>" << BLOCK_ARRAYS[currentArrayIdx].count << " | " << BLOCK_ARRAYS[currentArrayIdx].size << "</div>";
        }

        out << "<a title='BLOCK INFO'>";
        out << (priv::blocksUsed.Test(i) ? "&#9639;" : "&#9634;");
        out << "</a>";
    }

    out << "</div></body>";

}

}//ns

/* auto* blockType = blockTypes.GetValueOptional(i);
        
        out << "<a title='";
        out << (blockType == nullptr ? "unused" : *blockType);
        out << (blocksUsed.Test(i) ? "'>&#9639;</a>" : "'>&#9634;</a>"); //filled and not filled rect*/
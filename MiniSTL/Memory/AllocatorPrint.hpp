#pragma once
#include "MiniSTL/Memory/Allocator.hpp"

namespace mini::mem 
{
    inline void PrintHTML()
    {
        std::ofstream out("memory.html");
        out << R"(
<head>
    <style>
        * { padding:0; margin:0; border:0; font-size: 12px; font-family: monospace; }
        section div { position: absolute; height: 200px; border: 1px solid black; writing-mode: vertical-lr; text-orientation: upright;  }
        .used { background: red; }
        .free { background: green; }
        #mem-grid { width: 600px; word-wrap: break-word; height: 100%; border: 0; } 
    </style>
</head>
)";

        out << "<p>Total alloc: " << totalSize << " bytes</p>\n";
        out << "<p>Total used : " << TotalUsed() << " bytes</p>\n";
        out << "<p>Total free : " << TotalFree() << " bytes</p>\n";

        //basic view as bar 
        constexpr auto SCALE{ 20 }; //(4bytes = 4px) * SCALE
        out << "<section>";
        FOR_ARRAY(usedBlocks, i)
        {
            out
                << "<div class='used' style='left: "
                << ((usedBlocks[i].begin - base) * SCALE) //% 100
                //<< "px; top: "
                //<< (((usedBlocks[i].begin - base) * SCALE) / 100) * 64 + 64
                << "px; width: "
                << usedBlocks[i].Size() * SCALE
                << "px;'>"
                << usedBlocks[i].begin << "<br>"
                << usedBlocks[i].aligned
                << "</div>\n";
        }
        FOR_ARRAY(freeBlocks, i)
        {
            out
                << "<div class='free' style='left: "
                << ((freeBlocks[i].begin - base) * SCALE) //% 100
                //<< "px; top: "
                //<< (((freeBlocks[i].begin - base) * SCALE) / 100) * 64 + 64
                << "px; width: "
                << freeBlocks[i].Size() * SCALE
                << "px;'>"
                << freeBlocks[i].begin
                << "</div>\n";
        }
        out << "</section>";

        out << "<div id='mem-grid' style='position: absolute; top: 300px'>";
        for (auto i = 0; i < totalSize; ++i)
        {
            out << "&#9634;";
        }
        out << "</div>";

        out << "<script>var arr = new Array();";
        FOR_ARRAY(usedBlocks, i)
        {
            const auto idx = usedBlocks[i].begin - base;
            const auto num = usedBlocks[i].Size();
            out << "arr[" << i << "] = { idx: " << idx << ", num: " << num << "};";
        }
        out << R"(
let element = document.getElementById("mem-grid");
for(let i=0; i<arr.length; ++i) {
    var old = element.innerHTML;
    var newstr = old.substr(0,arr[i].idx);
    for(let j=0; j<arr[i].num; ++j) newstr += '&#9639;';
    newstr += old.substr(1+arr[i].idx+arr[i].num);
    element.innerHTML = newstr;
}
        )";
        out << "</script>";
    }

}//ns
#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>

//!this was used as standalone and called by main

inline void BMP_TO_RGBA(int argc, char *argv[])
{
    if (argc <= 1) {
        std::cout << "no path arg\n";
        system("pause");
        return;
    }

    char* path = argv[1];
    char* outputName = argv[2];
    std::cout << "path / dir "  << path << "\n";
    std::cout << "output name " << outputName << "\n";
    
    //reused buffer for all reads
    char* buffer = new char[10'000'000]; //! size must be enough
    std::ofstream out(outputName, std::ios::app);

    bool hasPrintedInfoOnce = false;
    for(auto& p: std::filesystem::directory_iterator(path)) 
    {
        std::cout << p.path() << '\n';

        std::ifstream file(p.path(), std::ios::ate | std::ios::binary);
        if(!file.is_open()) {
            std::cout << "cannot open\n";
            system("pause");
            return;
        }

        const auto size = file.tellg();
        file.seekg(std::ios::beg);
        file.read(buffer, size);

        //? BM 

        if (buffer[0] != 'B' && buffer[1] == 'M'){
            std::cout << "bad file";
            system("pause");
            return;
        }

        const auto bmSize     = *(int*)&buffer[2];
        const auto bmOffset   = *(int*)&buffer[10];
        const auto bmWidth    = *(int*)&buffer[18];
        const auto bmHeight   = *(int*)&buffer[22];
        const auto bmBitCount = *(int*)&buffer[28];

        if (!hasPrintedInfoOnce)
        {
            std::cout << bmSize << "\n";
            std::cout << bmOffset << "\n";
            std::cout << bmWidth << "\n";
            std::cout << bmHeight << "\n";
            std::cout << bmBitCount << "\n";
            hasPrintedInfoOnce = true;
        }

        if (bmWidth % 2 != 0 || bmHeight % 2 != 0){
            std::cout << "no power of 2 bmp";
            system("pause");
            return;
        }

        //append (mode)
        out.write(buffer + bmOffset, bmSize - bmOffset); 
    }
    delete[] buffer;

    system("pause");
}


/*
workflow

no need more for bmp to rgba, image magick can do it !

FONT TEXTURE ARRAY
- crop texture into tiles
- padding the texture to power of two
- name according to the ascii table -scene %03d
- convert to GRAY 
*/
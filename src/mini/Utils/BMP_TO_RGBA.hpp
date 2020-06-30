#pragma once
#include <iostream>
#include <fstream>

inline void BMP_TO_RGBA(int argc, char *argv[])
{
    if (argc <= 1) {
        std::cout << "no path arg\n";
        system("pause");
        return;
    }
    char* path = argv[1];
    std::cout << "path " << path << "\n";
    
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if(!file.is_open()) {
        std::cout << "cannot open\n";
        system("pause");
        return;
    }

    const auto size = file.tellg();
    std::cout << "size " << size << "\n";
    file.seekg(std::ios::beg);
    char* buffer = new char[10'000'000];
    std::cout << "read\n";
    file.read(buffer, size);

    //? BM HEADER CHECK

    if (buffer[0] != 'B' && buffer[1] == 'M'){
        std::cout << "bad file";
        system("pause");
        return;
    }

    std::cout << "getting data ...\n";

    const auto bmSize     = *(int*)&buffer[2];
    const auto bmOffset   = *(int*)&buffer[10];
    const auto bmWidth    = *(int*)&buffer[18];
    const auto bmHeight   = *(int*)&buffer[22];
    const auto bmBitCount = *(int*)&buffer[28];

    std::cout << bmSize << "\n";
    std::cout << bmOffset << "\n";
    std::cout << bmWidth << "\n";
    std::cout << bmHeight << "\n";
    std::cout << bmBitCount << "\n";

    if (bmWidth % 2 != 0 || bmHeight % 2 != 0){
        std::cout << "no power of 2 bmp";
        system("pause");
        return;
    }

    auto len = strlen(path);
    path[len-3] = 'r';
    path[len-2] = 'g';
    path[len-1] = 'b';

    std::cout << "out: " << path << " size " << bmSize - bmOffset;
    std::ofstream out(path);
    out.write(buffer + bmOffset, bmSize - bmOffset);

    delete[] buffer;

    system("pause");
}
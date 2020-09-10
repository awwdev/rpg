@echo off ^
 & mkdir build_optimized ^
 & mkdir build_optimized\objs ^
 & mkdir build_optimized\res ^
 & xcopy /s /y /d res\* build_optimized\res\* ^
 & Pushd G:\\DEV2020\\buildtools\\msys2\\mingw64\\bin ^
 & gcc ^
-I %2 ^
-L %5 ^
-Wall -Wpedantic -Wextra ^
-Ofast ^
-municode ^
-std=c++2a ^
-o %3 ^
%4 ^
-static ^
-lpthread ^
-lstdc++ ^
-lvulkan-1 ^
 & popd